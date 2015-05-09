
// Game data directory (either hosted on server, or in local html5 filesystem)
var ARX_DIR = 'arx';

// Main PAK File
// Only this file will be checked. If it is present, other PAK files are assumed to be here too.
var PAK_FILE = 'data.pak';

function updateStatus(message) {
  document.getElementById(STATUS_ID).innerHTML += message + '<br/>';  // STATUS_ID defined by html file
}

function runArx() {
  var embed = document.createElement('embed');
  // MODULE_* variables must be defined by the main html file
  embed.width = MODULE_WIDTH;
  embed.height = MODULE_HEIGHT;
  embed.type = MODULE_MIMETYPE;
  embed.src = MODULE_MANIFEST;
  embed.setAttribute('PS_STDOUT', "dev/console1");
  embed.setAttribute('PS_STDERR', "dev/console1");
  document.getElementById(MAIN_ID).appendChild(embed);   // MAIN_ID defined by html file
  
  function log_handler(tag) {
    return function () {
      var args = [tag].concat(Array.prototype.slice.call(arguments));
      console.log.apply(console, args);
    }
  }

  function error_handler() {
    console.error("Error occurred:", embed.lastError);
  }
  
  function crash_handler() {
    console.log("Exit code:", embed.exitStatus);
  }

  function progress_handler(evt) {
    if (evt.total != 0) {
      var progress = Number((evt.loaded * 100 / evt.total).toFixed(0));
      document.getElementById(LOADING_STATUS_ID).innerHTML = "Loading: " + progress + "%";  

      if (progress > 94) {
        document.getElementById(LOADING_STATUS_ID).innerHTML += "<br/>" + "Please, wait some more time while approaching 100%...";
      }
    }
  }

  function load_handler(evt) {
    document.getElementById(LOADING_STATUS_ID).innerHTML = "Loading: success.";
  }

  embed.addEventListener('loadstart', log_handler("loadstart"));
  embed.addEventListener('progress', log_handler("progress"));
  embed.addEventListener('progress', progress_handler);
  embed.addEventListener('load', log_handler("load"));
  embed.addEventListener('load', load_handler);
  embed.addEventListener('error', log_handler("error"));
  embed.addEventListener('abort', log_handler("abort"));
  embed.addEventListener('loadend', log_handler("loadend"));
  embed.addEventListener('message', log_handler("message"));
  embed.addEventListener('error', error_handler);
  embed.addEventListener('abort', error_handler);
  embed.addEventListener('crash', crash_handler);
}

function uploadDidChange(event) {
  
  function fileErrorHandler(e) {
    var msg = '';

    switch (e.code) {
      case FileError.QUOTA_EXCEEDED_ERR:
        msg = 'QUOTA_EXCEEDED_ERR';
        break;
      case FileError.NOT_FOUND_ERR:
        msg = 'NOT_FOUND_ERR';
        break;
      case FileError.SECURITY_ERR:
        msg = 'SECURITY_ERR';
        break;
      case FileError.INVALID_MODIFICATION_ERR:
        msg = 'INVALID_MODIFICATION_ERR';
        break;
      case FileError.INVALID_STATE_ERR:
        msg = 'INVALID_STATE_ERR';
        break;
      default:
        msg = 'Unknown Error';
        break;
    };

    console.log('Error: ' + msg);
  }
    
  window.webkitRequestFileSystem(window.TEMPORARY, 1024 * 1024 * 1024,
    function(fs) {
      fs.root.getDirectory(ARX_DIR, {create: true}, function(dirEntry) {
        var files = event.target.files;
        for (var i = 0, file; file = files[i]; ++i) {
          (function(f) {
            dirEntry.getFile(f.name, {create: true}, function(fileEntry) {
              fileEntry.createWriter(function(fileWriter) {
                function done(evt) {
                  updateStatus(f.name + ' copied to html5 filesystem.');                
                };
                function error(evt) {
                  updateStatus('Error while copying ' + f.name);
                };
                fileWriter.onwriteend = done
                fileWriter.write(f);
              });
            }, fileErrorHandler);
          })(file);
        }
      }, fileErrorHandler);
    }, fileErrorHandler);
}

function loadFromLocalStorage() {
  updateStatus('Searching for Arx data in local html5 filesystem (' + ARX_DIR + '/' + PAK_FILE + ').');
  
  function loadSuccess() {
    updateStatus('Found Arx data in local html5 filesystem. Launching arx.');
    runArx();
  }

  function loadFailure() {
    updateStatus('Arx data not found in local html5 filesystem.');
    updateStatus('Select game data (all PAK files and cfg_default.ini) to copy to local html5 filesystem.');
    updateStatus('');
    document.getElementById(MAIN_ID).innerHTML =
      '<input type="file" id="infile" multiple> \
       <input type="button" id="launch" value="Launch Arx">';
    document.getElementById('infile').addEventListener('change',
                                                       uploadDidChange,
                                                       false);
    document.getElementById('launch').addEventListener('click',
                                                       loadSuccess,
                                                       false);                                                       
  }

  window.webkitRequestFileSystem(window.TEMPORARY, 1024 * 1024 * 1024,
    function(fs) {
      fs.root.getDirectory(ARX_DIR, {}, function(dirEntry) {
        dirEntry.getFile(PAK_FILE, {}, loadSuccess, loadFailure) 
      }, loadFailure);
    }
  );
}

window.onload = function() {
  updateStatus('Searching for Arx data hosted on server (' + ARX_DIR + '/' + PAK_FILE + ').');
  var req = new XMLHttpRequest();
  req.onload = function() {
    if (this.status === 200) {
      updateStatus('Found Arx data on server. Launching arx.');
      runArx();
    } else {
      updateStatus('Arx data not found on server.');
      loadFromLocalStorage();
    }
  };
  req.open('HEAD', ARX_DIR + '/' + PAK_FILE);
  req.send(null);
};
