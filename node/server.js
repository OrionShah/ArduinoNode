var stdin = process.openStdin();
var io = require('socket.io');
var http = require('http');
var express = require('express');
var obj = require('./serialport_client')();

obj.init();

stdin.on('data', function (text) {
    obj.write(text);
});

var app = express();
app.use(express.static('./'));
var server = http.createServer(app);
var io = io.listen(server);
server.listen(8080);

io.sockets.on('connection', function (socket) {
    console.log(socket.conn.remoteAddress);
    socket.emit('data', obj.getInfo());
    socket.on('getData', function (data) {
        // console.log(socket.conn.remoteAddress+": ");
        socket.emit('data', obj.getInfo());
        socket.emit('resp', obj.getResp());
        socket.emit('led', obj.getLED());
        socket.emit('led1', obj.getLED1());
    });
    socket.on('write', function (text) {
        obj.write(text);
    });
    socket.on('disconnect', function () {
        console.log('user disconnected' + socket.conn.remoteAddress);
    });
});