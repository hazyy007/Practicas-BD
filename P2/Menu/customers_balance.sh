#!/usr/bin/expect -f

set timeout -1
spawn ./menu

expect "Enter a number that corresponds to your choice > "

send -- "3\r"

expect "Enter a number that corresponds to your choice > "

send -- "3\r"

expect "Write the coustumer number--> "

send -- "141\r"

expect -- "-104950.56"

puts "\n--------------------------------OK\n"

expect "Enter a number that corresponds to your choice > "

send -- "4\r"

expect "Enter a number that corresponds to your choice > "

send -- "4\r"

expect eof
