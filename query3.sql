SELECT e.employeenumber,
       e.lastname
FROM   employees e
       -- Relacionamos al empleado base (e) con su jefe directo/mánager (m)
       JOIN employees m
         ON e.reportsto = m.employeenumber
       -- Relacionamos al jefe directo (m) con el escalafón superior (d)
       JOIN employees d
         ON m.reportsto = d.employeenumber
-- Condición clave: Nos aseguramos de que el escalafón superior (d) es el director absoluto,
-- identificándolo porque no tiene a nadie por encima (su campo reportsto está vacío/nulo)
WHERE  d.reportsto IS NULL;
