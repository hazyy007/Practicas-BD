SELECT o.country,
       Count(*) AS numero_oficinas
FROM   offices o
WHERE  NOT EXISTS (SELECT *
                   FROM   employees e,
                          customers c,
                          orders ord,
                          orderdetails od
                   WHERE  e.officecode = o.officecode
                          AND c.salesrepemployeenumber = e.employeenumber
                          AND ord.customernumber = c.customernumber
                          AND od.ordernumber = ord.ordernumber
                          AND ord.orderdate >= '2003-01-01'
                          AND ord.orderdate < '2004-01-01')
GROUP  BY o.country
ORDER  BY Count(*) DESC;  