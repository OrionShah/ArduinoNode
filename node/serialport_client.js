var serialPort = require("serialport");

var sp = function () {
    var self = this;
    this.port;
    this.btn_state = 0;
    this.res = '';
    this.led = 0;
    this.led1 = 0;
    this.init = function () {
        serialPort.list(function (err, ports) {
            var regexp = /Arduino/i;
            ports.forEach(function(port) {
                if (regexp.test(port.pnpId)) {
                    // console.log(port);
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
                    self.getData(data);
                });
            }
        });
    }

    this.write = function (text) {
        this.port.write(text);
    };

    this.getData = function (data) {
        var btn_regex = /Button: ([0-9]+)/i;
        var res_regex = /You say: ([A-z 0-9]+)/i;
        var led_regex = /led_fade: ([0-9]+)/i;
        var led1_regex = /led1_fade: ([0-9]+)/i;
        if (btn_regex.test(data)) {
            var btn = btn_regex.exec(data);
            self.btn_state = btn[1];
            // console.log(self.btn_state);
        } else {
            // console.log(data);
        }

        if (res_regex.test(data)){
            var res = res_regex.exec(data);
            self.res = data;
            console.log(data);
        }

        if (led_regex.test(data)) {
            var res = led_regex.exec(data);
            self.led = res[1];
        }

        if (led1_regex.test(data)) {
            var res = led1_regex.exec(data);
            self.led1 = res[1];
        }
        // console.log(data);
    };

    this.getInfo = function () {
        return "Button: " + self.btn_state;
    };

    this.getResp = function () {
        return self.res;
    };

    this.getLED = function () {
        return self.led;
    };

    this.getLED1 = function () {
        return self.led1;
    };

    return this;
}

module.exports = sp;