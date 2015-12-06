#!/usr/bin/env node
'use strict';

// Global depedencies
var program = require('commander'),
    colors = require('colors'),
    Firebase = require('firebase');

// local dependencies
var pkg = require('./package.json'),
    UDPBridge = require('./lib/udpbridge.js');

var listenPort = 11000;

function checkStartupParameters() {
    program
        .version(pkg.version)
        .option('-p, --port [number]', 'specifies the port (default: 8999)')
        .parse(process.argv);

    if (!isNaN(parseFloat(program.port)) && isFinite(program.port)){
        listenPort = program.port;
    }
}

function checkForUpgrade() {
    require('check-update')({packageName: pkg.name, packageVersion: pkg.version, isCLI: true}, function(err, latestVersion, defaultMessage){
        if(!err){
            console.log(defaultMessage);
        }
    });

    require('check-update-github')({name: pkg.name, currentVersion: pkg.version, user: lukevenediger, branch: 'master'}, function(err, latestVersion, defaultMessage){
        if(!err){
            console.log(defaultMessage);
        }
    });
}

function startService() {
    console.log('Server running at\n  => ' + colors.green('http://localhost:' + port) + '\nCTRL + C to shutdown');
    new UDPBridge(port);
}

checkStartupParameters();
checkForUpgrade();
startService();
