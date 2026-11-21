#include <stdlib.h>
#include <string.h>
#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

 /* Descripcion: Reemplaza la extension de fileName por ".idx" y guarda el resultado en indexName.*/
void replaceExtensionByIdx(const char *fileName, char *indexName) {
    char *dot = NULL;

    if (fileName == NULL || indexName == NULL) {
        return;
    }

    strcpy(indexName, fileName);
    dot = strrchr(indexName, '.');
    if (dot != NULL) {
        strcpy(dot, ".idx");
    } else {
        strcat(indexName, ".idx");
    }
}

/*Descripcion: Crea el fichero de indices si no existe, inicializando root y deleted a -1.*/
bool createIndex(const char *indexName) {
    FILE *fp = NULL;

    if (indexName == NULL) {
        return false;
    }

    fp = fopen(indexName, "rb");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }

    fp = fopen(indexName, "wb");
    if (fp == NULL) {
        return false;
    }

    /* Escribimos root = -1 y deleted = -1 (8 bytes en total) */
    if (fwrite(&no_deleted_registers, sizeof(int), 1, fp) != 1 ||
        fwrite(&no_deleted_registers, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

/*Descripcion: Crea el fichero de datos si no existe (cabecera a -1) y llama a createIndex.*/
bool createTable(const char *tableName) {
    FILE *fp = NULL;
    char *indexName = NULL;
    bool res = false;

    if (tableName == NULL) {
        return false;
    }

    fp = fopen(tableName, "rb");
    if (fp != NULL) {
        fclose(fp);
    } else {
        fp = fopen(tableName, "wb");
        if (fp == NULL) {
            return false;
        }
        /* Cabecera de 4 bytes con -1 (sin registros borrados) */
        if (fwrite(&no_deleted_registers, sizeof(int), 1, fp) != 1) {
            fclose(fp);
            return false;
        }
        fclose(fp);
    }

    /* Reservamos memoria para el nombre del .idx (+5 para ".idx" y '\0') */
    indexName = (char *)malloc(strlen(tableName) + 5);
    if (indexName == NULL) {
        return false;
    }

    replaceExtensionByIdx(tableName, indexName);
    res = createIndex(indexName);

    free(indexName);
    return res;
}

void printTree(size_t level, const char *indexName) {
    (void)level;
    (void)indexName;
    return;
}

bool findKey(const char *book_id, const char *indexName,
             int *nodeIDOrDataOffset) {
    (void)book_id;
    (void)indexName;
    (void)nodeIDOrDataOffset;
    return false;
}

bool addIndexEntry(char *book_id, int bookOffset, const char *indexName) {
    (void)book_id;
    (void)bookOffset;
    (void)indexName;
    return false;
}

bool addTableEntry(Book *book, const char *tableName,
                   const char *indexName) {
    (void)book;
    (void)tableName;
    (void)indexName;
    return false;
}