 SELECT e.employeenumber,
       e.lastname
FROM   employees e
       JOIN employees m
         ON e.reportsto = m.employeenumber
       JOIN employees d
         ON m.reportsto = d.employeenumber
WHERE  d.reportsto IS NULL;  

