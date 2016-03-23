var socket = io.connect('http://192.168.1.109:8080');
socket.on('data', function (data) {
    $('#data').html(data);
    socket.emit('getData');
});

socket.on('resp', function (data) {
    $('#resp').html(data);
});
socket.on('led', function (data) {
    $('#led').html('LED: '+data);
});
socket.on('led1', function (data) {
    $('#led1').html('LED1: '+data);
});

$('#send').click(function(event) {
    var text = $('#text').val();
    // console.log('text '+text);
    socket.emit('write', text);
    return false;
});