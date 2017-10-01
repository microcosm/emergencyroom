var fs = require('fs');

var settingsFile = fs.readFileSync('../bin/data/settings.json').toString();
var settings = JSON.parse(stripJsonComments(settingsFile));

var toPrint = settings['machine-names'].server
console.log(toPrint);

function stripJsonComments(str){
	return str.replace(/(\/\*([\s\S]*?)\*\/)|(\/\/(.*)$)/gm, '');
}