SELECT od1.productcode        AS Producto_1,
       od2.productcode        AS Producto_2,
       Count(od1.ordernumber) AS numero_carros
FROM   orderdetails od1
       JOIN orderdetails od2
         ON od1.ordernumber = od2.ordernumber
            AND od1.productcode < od2.productcode
GROUP  BY od1.productcode,
          od2.productcode
HAVING Count(od1.productcode) > 1
ORDER  BY numero_carros DESC; 