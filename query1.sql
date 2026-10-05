SELECT  c.customernumber AS "Id comprador",
        c.customername   AS "Nombre comprador",
        SUM(p.amount)    AS "Cantidad Total"  -- Sumamos el importe de todos los pagos de ese cliente
FROM    customers c
        -- Hacemos JOIN con payments para poder acceder a los importes de los pagos
        JOIN payments p ON c.customernumber = p.customernumber
-- Filtramos usando una subconsulta: nos quedamos solo con clientes cuyo ID esté en la lista de compradores del Ford
WHERE   c.customernumber IN (
            SELECT  o.customernumber
            FROM    orders o
                    JOIN orderdetails od ON o.ordernumber = od.ordernumber
                    JOIN products pr ON od.productcode = pr.productcode
            WHERE   pr.productname = '1940 Ford Pickup Truck'
        )
-- Agrupamos por los datos del cliente para poder aplicar la función de agregación SUM() correctamente
GROUP BY c.customernumber, c.customername
-- Ordenamos el resultado de mayor a menor cantidad total pagada
ORDER BY "Cantidad Total" DESC;