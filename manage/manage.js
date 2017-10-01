var fs = require('fs');
var os = require('os');
var http = require('http');
var settings, machineType, port;

run();

function run(){
	init();
	console.log('This system is ' + machineType + '.');
}

/* init */
function init(){
	readSettings();
	setPort();
	setMachineType();
	if(isOFClient()){
		startManagementServer();
	}
}

function readSettings(){
	var settingsFile = fs.readFileSync('../bin/data/settings.json').toString();
	settings = JSON.parse(stripJsonComments(settingsFile));
}

function setPort(){
	port = settings['manage-port'];
}

function setMachineType(){
	var hostname = os.hostname();
	if(hostname == settings['machine-names'].server){
		machineType = 'server';
	}else if(hostname == settings['machine-names'].ecg){
		machineType = 'ECG';
	}else{
		machineType = 'client';
	}
}

/* server */
function startManagementServer(){
	const requestHandler = (request, response) => {
		var action = request.url.includes('startOF') ? 'Starting openFrameworks app now.' : 'Unrecognized request.';
		console.log('Recieved \'' + request.url + '\', responding with \'' + action + '\'');
		response.end(action);
	}

	const server = http.createServer(requestHandler)

	server.listen(port, (err) => {
		if(err){
			return console.log('Error caught: ', err)
		}

		console.log('Management server is listening on ' + port + '...');
	});
}

/* util */
function stripJsonComments(str){
	return str.replace(/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/gm, '');
}

function isOFClient(){
	return machineType != 'server';
}