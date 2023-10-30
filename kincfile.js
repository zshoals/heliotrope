const fs = require('fs');
const path = require("path");

let project = new Project('Heliotrope');
project.addFile('Sources/**');
project.addFiles('Sources/shaders/**');
project.addFiles('Sources/assets/**');
project.addIncludeDir('Sources');
project.setDebugDir('Deployment');

resolve(project);