SELECT pr.productline,
       -- Calculamos la diferencia entre la fecha de envío y la de pedido, y sacamos el promedio
       Avg(o.shippeddate - o.orderdate) AS "Tiempo medio"
FROM   orders o
       -- Relacionamos la cabecera del pedido con sus detalles y, finalmente, con el catálogo de productos
       JOIN orderdetails od ON o.ordernumber = od.ordernumber
       JOIN products pr ON od.productcode = pr.productcode
-- Es crucial descartar los pedidos que aún no han sido enviados para no alterar la media con nulos
WHERE  o.shippeddate IS NOT NULL
-- Agrupamos los resultados por la familia del producto
GROUP BY pr.productline;