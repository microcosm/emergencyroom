var fs = require('fs');
var os = require('os');
var machineType = 'not set';

run();

function run(){
	init();
	console.log('This system is ' + machineType + '.');
}

function init(){
	var settingsFile = fs.readFileSync('../bin/data/settings.json').toString();
	var settings = JSON.parse(stripJsonComments(settingsFile));

	var hostname = os.hostname();
	if(hostname == settings['machine-names'].server){
		machineType = 'server';
	}else if(hostname == settings['machine-names'].ecg){
		machineType = 'ECG';
	}else{
		machineType = 'client';
	}
}

function stripJsonComments(str){
	return str.replace(/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/gm, '');
}