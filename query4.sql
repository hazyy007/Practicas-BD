SELECT e.officecode            AS codigo_oficina,
       Sum(od.quantityordered) AS productos_vendidos
FROM   orderdetails od
       JOIN orders o
         ON od.ordernumber = o.ordernumber
       JOIN customers c
         ON o.customernumber = c.customernumber
       JOIN employees e
         ON c.salesrepemployeenumber = e.employeenumber
GROUP  BY e.officecode
ORDER  BY productos_vendidos DESC
LIMIT  1; 