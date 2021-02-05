const path = require('path');
const express = require('express');
const WebSocket = require('ws');
const app = express();
var telemetry = require('./telemetryData.js');

const WS_PORT = 8888;
const HTTP_PORT = 8000;

const wsServer = new WebSocket.Server({ port: WS_PORT }, () => console.log(`WS Server is listening at ${WS_PORT}`));

let connectdClients = [];
wsServer.on('connection', (ws, req) => {
    console.log('Connected');
    connectdClients.push(ws);
    ws.on('message', data => {
        telemetry.addEnvReport(data)
        connectdClients.forEach((ws, i) => {
            if (ws.readyState === ws.OPEN) {
                ws.send(data)
            } else {
                connectdClients.splice(i, 1);
            }
        })
    });
    setInterval(function(){
        ws.send('just checking');
    },100000)
});
app.get('/client', (req, res)=> res.sendFile(path.resolve(__dirname, './client.html')));
// app.get('/data', (req, res)=> res.sendFile(path.resolve(__dirname, './data.json')));
app.get('/env', (req, res)=> res.sendFile(path.resolve(__dirname, './environmentData.json')));

var now = new Date(); // 21 = 9PM, 0-23 range
app.get('/data', (req, res)=> res.send('{"start":"4:00:00","end":"21:00:00","current":' + (now.getHours()+2) + ',"frequency": 600, "salutation":"howdy" }'));

app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));