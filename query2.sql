SELECT pr.productline,
       Avg(o.shippeddate - o.orderdate) AS "Tiempo medio"
FROM   orders o
       JOIN orderdetails od
         ON o.ordernumber = od.ordernumber
       JOIN products pr
         ON od.productcode = pr.productcode
WHERE  o.shippeddate IS NOT NULL
GROUP  BY pr.productline 