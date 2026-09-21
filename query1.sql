SELECT c.customernumber as "Id comprador", c.customername as "Nombre comprador", SUM(p.amount) as "Cantidad Total"
FROM customers c
JOIN payments p ON c.customernumber = p.customernumber
WHERE c.customernumber IN (
    SELECT o.customernumber
    FROM orders o
    JOIN orderdetails od ON o.ordernumber = od.ordernumber
    JOIN products pr ON od.productcode = pr.productcode
    WHERE pr.productname = '1940 Ford Pickup Truck'
)
GROUP BY c.customernumber, c.customername
ORDER BY "Cantidad Total" DESC;
