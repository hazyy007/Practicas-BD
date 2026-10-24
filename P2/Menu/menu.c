#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

/* Prototipos actualizados */
static int ShowMainMenu();
static void ShowProductsMenu(SQLHDBC dbc);
static int ShowProductsSubMenus();
static void StockQuery(SQLHDBC dbc);
static void FindQuery(SQLHDBC dbc);
static void ShowOrdersMenu(SQLHDBC dbc);
static int ShowOrdersSubMenu();
static void OpenQuery(SQLHDBC dbc);
static void RangeQuery(SQLHDBC dbc);
static void DetailQuery(SQLHDBC dbc);
static void ShowCustomersMenu();

int main(void) {
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    int ret;
    int nChoice = 0;

    ret = odbc_connect(&env, &dbc); /*Conectamos la bd*/
    if (!SQL_SUCCEEDED(ret)) {
        fprintf(stderr, "Error conectando a la base de datos.\n");
        return EXIT_FAILURE;
    }
    do {
        nChoice = ShowMainMenu();
        switch (nChoice) {
            case 1:
                ShowProductsMenu(dbc);
                break;
            case 2:
                ShowOrdersMenu(dbc);
                break;
            case 3:
                ShowCustomersMenu();
                break;
            case 4:
                printf("Bye Bye\n\n");
                break;
        }
    } while (nChoice != 4); /* El bucle sale con la opción 4 */
    odbc_disconnect(env, dbc); /*Desconectamos la bd*/
    return 0;
}

int ShowMainMenu() {
    int nSelected = 0;
    char buf[16]; 

    do {
        printf("This is a classicmodels database program\n");
        printf("You can choose to display different information\n\n");
        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
               
        if (!fgets(buf, 16, stdin))
            nSelected = 0;
        else
            nSelected = atoi(buf);
            
        printf("\n");
        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));
    
    return nSelected;
}


static void ShowProductsMenu(SQLHDBC dbc) {
    int nChoice = 0;
    do{
        nChoice = ShowProductsSubMenus();
        switch (nChoice)
        {
        case 1:
            StockQuery(dbc);
            break;
        case 2:
            FindQuery(dbc);
            break;
        default:
            printf("Bye Bye\n\n");
            break;
        }
    }while (nChoice != 3); 
}

int ShowProductsSubMenus()
{
    int nSelect = 0;
    char buf[16];
    do
    {
        printf("Product submenu\n");
        printf(" (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin)) nSelect = 0;
        else nSelect = atoi(buf);
        printf("\n");
        if (nSelect < 1 || nSelect > 3)
        {
            printf("Your choice is out of range. Please try agin\n\n\n");
        }

    } while ((nSelect < 1) || (nSelect > 3));
    return nSelect;
}

/*CONSULTA 1: STOCK*/
static void StockQuery(SQLHDBC dbc)
{
    SQLHSTMT stmt = NULL;
    char productcode[50];
    SQLINTEGER stock = 0;

    printf("Enter productcode: ");
    if (fgets(productcode, sizeof(productcode), stdin) != NULL)
    {
        productcode[strcspn(productcode, "\n")] = 0; /*Fuera el salto de linea */
    }

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "SELECT quantityinstock FROM products WHERE productcode = ?", SQL_NTS);
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, productcode, sizeof(productcode), NULL);
    SQLExecute(stmt);
    /*Vinculamos*/
    SQLBindCol(stmt, 1, SQL_C_SLONG, &stock, 0, NULL);
    /*Guardamos el resultado*/
    if (SQL_SUCCEEDED(SQLFetch(stmt))) {
        printf("\n[RESULT] Stock for %s: %d units.\n\n", productcode, stock);
    } else {
        printf("\n[ERROR] Product code '%s' not found.\n\n", productcode);
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

static void FindQuery(SQLHDBC dbc)
{
    SQLHSTMT stmt = NULL;
    char search_input[100];
    char search_pattern[104]; /* Buffer para añadir los % al inicio y final */
    char productcode[50] = "\0";
    char productname[100] = "\0";
    SQLRETURN ret;
    int count = 0;

    printf("Enter product name to search: ");
    if (fgets(search_input, sizeof(search_input), stdin) != NULL) {
        search_input[strcspn(search_input, "\n")] = 0; /* Limpiar salto de línea */
    }

    /* Añadimos los comodines % para que LIKE busque subcadenas */
    sprintf(search_pattern, "%%%s%%", search_input);

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    /* PREPARED STATEMENT OBLIGATORIO: Ordenado por productcode como pide la práctica */
    SQLPrepare(stmt, (SQLCHAR*) "SELECT productcode, productname FROM products WHERE productname LIKE ? ORDER BY productcode", SQL_NTS);
    
    SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, search_pattern, sizeof(search_pattern), NULL);
    SQLExecute(stmt);

    /* Vinculamos las dos columnas que devuelve el SELECT */
    SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*)productcode, sizeof(productcode), NULL);
    SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*)productname, sizeof(productname), NULL);

    printf("\n--- Results for '%s' ---\n", search_input);

    
    /* Bucle para imprimir todas las filas devueltas por la consulta */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\t%s\n", productcode, productname);
        count++;
    }
    
    if (count == 0) {
        printf("No products found containing '%s'.\n", search_input);
    }
    printf("------------------------\n\n");

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

static void ShowOrdersMenu(SQLHDBC dbc) {
     int nChoice = 0;
    do{
        nChoice = ShowOrdersSubMenu();
        switch (nChoice)
        {
        case 1:
            OpenQuery(dbc);
            break;
        case 2:
            RangeQuery(dbc);
            break;
        case 3:
            DetailQuery(dbc);
            break;
        default:
            printf("Bye Bye\n\n");
            break;
        }
    }while (nChoice != 4); 
}

int ShowOrdersSubMenu()
{
    int nSelect = 0;
    char buf[16];
    do
    {
        printf("Orders submenu\n");
        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin)) nSelect = 0;
        else nSelect = atoi(buf);
        printf("\n");
        if (nSelect < 1 || nSelect > 3)
        {
            printf("Your choice is out of range. Please try agin\n\n\n");
        }

    } while ((nSelect < 1) || (nSelect > 4));
    return nSelect;
}

static void OpenQuery(SQLHDBC dbc)
{
    /*Codigo SQL:
    SELECT ordernumber FROM orders WHERE shippdate is NULL order by ordernumber*/
    SQLHSTMT stmt = NULL;
    SQLINTEGER ordernumber = 0;
    int count = 0;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

    SQLPrepare(stmt, (SQLCHAR*) "SELECT ordernumber FROM orders WHERE shippeddate IS NULL ORDER BY ordernumber", SQL_NTS);
    SQLExecute(stmt);
    /*Vinculamos*/
    SQLBindCol(stmt, 1, SQL_C_SLONG, &ordernumber, 0, NULL);
    /*Guardamos el resultado*/
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%d.\n\n", ordernumber);
        count++;
    } if (count == 0) {
        printf("\n[ERROR] All orders have been shalled");
    }

    printf("\n-------------------------\n");

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
}

static void RangeQuery(SQLHDBC dbc)
{
    SQLHSTMT stmt = NULL;
    SQLINTEGER ordernumber;
    char orderdate[15] = "\0";
    char shippeddate[15] = "\0";
    SQLLEN shippeddate_ind = 0;
    char input[50];
    char first_date[11] = "\0";
    char second_date[11] = "\0";
    SQLRETURN ret;
    int count = 0;

/*input doble de las fechas*/
    printf("Enter the range of the dates (YYYY-MM-DD - YYYY-MM-DD): ");

    /*Guardamos toda la linea a la vez*/
    if (fgets(input, sizeof(input), stdin) != NULL)
    {
        /*Ahora separamos las fechas*/
        if (sscanf(input, "%10s - %10s", first_date, second_date) == 2)
        {
            printf("Buscando pedidos entre %10s y %10s", first_date, second_date);
            SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

            /*Ponemos el cpdigo SQL*/
            SQLPrepare(stmt, (SQLCHAR*) "SELECT ordernumber, orderdate, shippeddate FROM orders WHERE orderdate BETWEEN ? AND ? ORDER BY ordernumber", SQL_NTS);
            
            SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, first_date, sizeof(first_date), NULL);
            SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, second_date, sizeof(second_date), NULL);
            SQLExecute(stmt);

            /* Vinculamos las dos columnas que devuelve el SELECT */
            SQLBindCol(stmt, 1, SQL_C_LONG, &ordernumber, 0, NULL);
            SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*)orderdate, sizeof(orderdate), NULL);
            SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*)shippeddate, sizeof(shippeddate), &shippeddate_ind);

            printf("\n--- Results in range '%10s' and '%10s' ---\n", first_date, second_date);

            
            /* Bucle para imprimir todas las filas devueltas por la consulta */
            while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
                if (shippeddate_ind == SQL_NULL_DATA){
                    printf("%d\t%s\n", ordernumber, orderdate);
                }
                else{
                    printf("%d\t%s\t%s\n", ordernumber, orderdate, shippeddate);
                }
                count++;
            }
            
            if (count == 0) {
                printf("No orders found in that range.\n");
            }
            printf("------------------------\n\n");

            SQLCloseCursor(stmt);
            SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        }
    }
    
}

static void DetailQuery(SQLHDBC dbc)
{
    /*codigo SQL:
        SELECT 
            o.orderdate, 
            o.status, 
            od.productcode, 
            od.quantityordered, 
            od.priceeach, 
            (SELECT SUM(quantityordered * priceeach) FROM orderdetails WHERE ordernumber = o.ordernumber) AS Precio_total
        FROM orders o
        JOIN orderdetails od ON o.ordernumber = od.ordernumber
        WHERE o.ordernumber = 10100
        ORDER BY od.orderlinenumber;*/

        printf("En proceso");
}

static void ShowCustomersMenu() {
    printf("--- Submenú Customers (En construcción) ---\n\n");
}