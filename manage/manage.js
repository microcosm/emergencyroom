var fs = require('fs');
var os = require('os');
var http = require('http');
var spawn = require('child_process').spawn;
var exec = require('child_process').exec;
var settings, machineType, port, runProc, compileProc;
var started = false;

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
	const server = http.createServer(requestHandler)
	server.listen(port, function(err){
		if(err){
			return console.log('Error caught: ', err);
		}

		console.log('Management server is listening on ' + port + '...');
	});
}

function requestHandler(request, response){
	var action = interpretRequest(request.url);
	var responseStr = 'Recieved \'' + request.url + '\', interpreted action \'' + action + '\'';
	console.log(responseStr);

	switch(action){
		case 'start':
			startOF();
			break;
		case 'stop':
			stopOF();
			break;
		case 'compile':
			compileOF();
			break;
	}

	response.end(responseStr);
}

function interpretRequest(url){
	if(url.includes('startOF')){
		return 'start';
	}else if(url.includes('stopOF')){
		return 'stop';
	}else if(url.includes('compileOF')){
		return 'compile';
	}else{
		return 'unknown';
	}
}

/* command execution */
function startOF(){
	if(!started){
		started = true;
		spawnCmd('../bin/emergencyroom', runProc);
	}
}

function stopOF(){
	killRunningApp();
	exec('sudo reboot');
}

function compileOF(){
	killRunningApp();
	spawnCmd('/usr/bin/make -j2', compileProc);
}

function killRunningApp(){
	if(started){
		runProc.kill('SIGINT');
		started = false;
	}
}

function spawnCmd(cmd, proc){
	proc = spawn(cmd, {cwd: '/home/pi/emergency-room/of098/apps/myApps/emergencyroom'});

	proc.stdout.on('data', function(data) {
	  console.log('stdout: ' + data.toString());
	});

	proc.stderr.on('data', function(data) {
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