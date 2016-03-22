var stdin = process.openStdin();
var serialPort = require("serialport");

var sp = function () {
    this.port;
    this.init = function () {
        serialPort.list(function (err, ports) {
            var regexp = /Arduino/i;
            ports.forEach(function(port) {
                //console.log(port.comName);
                if (regexp.test(port.pnpId)) {
                    this.createPort(port.comName);
                    this.createListener();
                    // console.log(port.comName + ' 2');
                    // console.log(port.pnpId);
                    // console.log(port.manufacturer);
                }
            });
        });
    };

    this.createPort = function (id) {
        this.port = new serialPort.SerialPort(id, {
            baudrate: 9600,
            parser: serialPort.parsers.readline('\r\n')

        });
    };

    this.createListener = function () {
        this.port.open(function (error) {
            if ( error ) {
                console.log('failed to open: '+error);
            } else {
                console.log('open');
                this.port.on('data', function(data) {
                    console.log(''+data);
                });
                
            }
        });
    }

    this.write = function (text) {
        this.port.write(text);
    }

    return this;
}


var obj = sp();

obj.init();


stdin.on('data', function (text) {
    obj.write(text);
});