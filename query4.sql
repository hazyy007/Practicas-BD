SELECT e.officecode AS codigo_oficina,
       Sum(od.quantityordered) AS productos_vendidos -- Sumamos las cantidades de cada línea de pedido
FROM   orderdetails od
       JOIN orders o ON od.ordernumber = o.ordernumber
       JOIN customers c ON o.customernumber = c.customernumber
       -- Necesitamos llegar a la tabla employees para localizar la oficina del representante de ventas
       JOIN employees e ON c.salesrepemployeenumber = e.employeenumber
-- Agrupamos todo por el código de la oficina para totalizar sus ventas
GROUP BY e.officecode
-- Ordenamos de manera descendente para que la oficina con más ventas quede arriba
ORDER BY productos_vendidos DESC
-- Nos quedamos exclusivamente con la primera tupla (la oficina que más ha vendido)
LIMIT  1;