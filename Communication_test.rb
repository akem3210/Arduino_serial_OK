#!/usr/bin/env ruby
require "serialport"
port_str = "/dev/ttyUSB0"
baud_rate = 115200 # default 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

sp = SerialPort.new(port_str, baud_rate, data_bits, stop_bits, parity)
sp.read_timeout = 1000
#sp.write_timeout = 1000

puts "Arduino must be loaded with the Serial_OK program and connected on USB."
puts "This program send a command {cmd=X} to the arduino which replies back as echo."
puts "Waiting 5 seconds before starting..."
0.upto(4){ |i|
	puts "* #{i} *"
	sleep 1
}

puts "-GO!-"

i = 0
while true do
  cmd = "{ANIMATRONIC=#{i}}"
  puts "cmd=\"#{cmd}\""
  sp.write(cmd)

  message = sp.gets
  if(message) then
    message.chomp!
    puts "echoed=\"#{message}\""
  end
  
  i += 1
  if(i == 100) then i = 0; end
  
  sp.flush_output
  sp.flush_input
  
#  sleep 0.1 #5
  # in fact we break at 99 for now
  if(i == 99) then break; end
end
=begin
0.upto(25){
message = sp.gets
  if(message) then
    message.chomp!
    puts "post=\"#{message}\""
  end
}
=end
sp.flush_output
sp.flush_input
sp.close
