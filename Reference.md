# Introduction #

Master funcrtions


## modbus\_open ##
modbus\_open(string **$port**, int **$baud\_rate**, char **$parity**, int **$bit\_stop**, int **$time\_out**);<br>
<i>example</i>: $port = modbus_open("/dev/ttyUSB0", 38400, "N", 2, 50);<br>
<br>
<h2>modbus_close</h2>
<i>use</i> : modbus_close(ref $port);<br>
$port = reference of modbus_open call<br>
<br>
<h2>modbus_read_holding_registers</h2>
<i>use</i> : $v = modbus_read_holding_registers(ref <b>$port</b>, int <b>$id_slave</b>, int <b>$register</b>, int <b>$n_read</b>);<br>
$port = reference of modbus_open call<br>
$id_slave = number Id Node of the slave device<br>
$register = Register number start to read<br>
$n_read = how many registers reads (started from $register)<br>
<br>
<i>example</i>:<br>
$v = modbus_read_holding_registers($port, 28, 215, 4); // <i>read registers: 215, 216, 217 and 218</i>

<h2>modbus_read_input_registers</h2>
<i>use</i> : $v = modbus_read_input_registers(ref $port, int $id_slave, int $Register, int $nRead);<br>
<i>read above</i>

<h2>modbus_write</h2>
$r = modbus_write(int port, unsigned int device, unsigned int add, unsigned int len, unsigned int <b>dataw);</b><br>
(Sorry, we have to improve this last one)