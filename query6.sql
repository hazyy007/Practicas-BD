SELECT od1.productcode AS Producto_1,
       od2.productcode AS Producto_2,
       Count(od1.ordernumber) AS numero_carros -- Contamos en cuántos pedidos distintos coinciden ambos productos
FROM   orderdetails od1
       -- Cruzamos la tabla consigo misma buscando el mismo número de pedido
       JOIN orderdetails od2 ON od1.ordernumber = od2.ordernumber
       -- Condición clave: '<' evita que un producto se empareje consigo mismo 
       -- y también elimina las parejas repetidas
       AND od1.productcode < od2.productcode
-- Agrupamos por la pareja única formada
GROUP BY od1.productcode, od2.productcode
-- Filtramos después de agrupar para descartar las parejas que solo han coincidido 1 vez
HAVING Count(od1.productcode) > 1
-- Ordenamos de mayor a menor
ORDER BY numero_carros DESC;