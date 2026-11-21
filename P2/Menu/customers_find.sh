#!/usr/bin/expect -f
set timeout -1
spawn ./menu

expect "Enter a number that corresponds to your choice > "

send -- "3\r"

expect "Enter a number that corresponds to your choice > "

send -- "1\r"

expect "Enter customer name > "

send -- "Mary\r"
expect "Saveley & Henriot, Co.\tMary \tSaveley\t146"

expect "Boards & Toys Co.\tMary\tYoung\t219"

expect "\[ < Prev | > Next | q Quit \] > "

send -- "q\r"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "

send -- "4\r"

expect "Enter a number that corresponds to your choice > "

send -- "4\r"

expect eof