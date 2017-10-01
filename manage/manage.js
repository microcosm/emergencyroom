var fs = require('fs');
var os = require('os');
var http = require('http');
var spawn = require('child_process').spawn;
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
	const requestHandler = function(request, response){
		var isStartRequest = request.url.includes('startOF');
		var action = isStartRequest ? 'Starting openFrameworks app now.' : 'Unrecognized request.';
		console.log('Recieved \'' + request.url + '\', responding with \'' + action + '\'');
		if(isStartRequest){
			startOF();
		}
		response.end(action);
	}

	const server = http.createServer(requestHandler)

	server.listen(port, function(err){
		if(err){
			return console.log('Error caught: ', err);
		}

		console.log('Management server is listening on ' + port + '...');
	});
}

/* command execution */
function startOF(){
	var command = spawn('../bin/emergencyroom');

	command.stdout.on('data', function(data) {
	  console.log('stdout: ' + data.toString());
	});

	command.stderr.on('data', function(data) {
	  console.log('stderr: ' + data.toString());
	});
}

/* util */
function stripJsonComments(str){
	return str.replace(/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/gm, '');
}

function isOFClient(){
	return machineType != 'server';
}