
var timeZone="CST";
var dateTimeFormat="dd/MM/yyyy HH:mm:ss";
var logSpreadSheetId="";

function sendEmail(message, id) {
    var subject = 'Something wrong with ' + id;
    MailApp.sendEmail(emailAddress, subject, message);

}

function doGet(e) {
   var access="-1";
      var text='go home';
      var name='Who are you?';
  var json;
  var error="idk";
    Logger.log(JSON.stringify(e)); // view parameters
    var result = 'Ok'; // assume success
    if (e.parameter == 'undefined') {
        result = 'No Parameters';
    } else {

        var uid = '';
        var onlyPing=false;
        var id = 'headquarter';
		var newvar='';
        var error = '';
        for (var param in e.parameter) {

            var value = stripQuotes(e.parameter[param]);

            switch (param) {
                case 'uid':
                    uid = value;
                    break;
					case 'newvar':
                    newvar = value;
					 break;
                case 'id':
                    id = value;
                    break;
       
                default:
                    result = "unsupported parameter";
            }
        }
      
     
      var sheet=SpreadsheetApp.getActive().getActiveSheet();
     
      var data = sheet.getDataRange().getValues();
    if (data.length == 0)
        return;
    for (var i = 0; i < data.length; i++) {

        if (data[i][0] ==uid)
        {
          name=data[i][1];
          access=data[i][2];
          text=data[i][3];
          break;
        }
       
    }
      
      
      addLog(uid,id,name,access,newvar);
       
    
       }
//     json = {
//    'access':access, 
//    'name': name,
//    'text':text,     
//    'error':error}
     
     result=(access+":"+name+":"+text);
  return ContentService.createTextOutput(result);
//  return ContentService.createTextOutput(JSON.stringify(json) ).setMimeType(ContentService.MimeType.JSON); 
}


function addLog(uid,entrance, name,result,newvar) {

    
    var spr=SpreadsheetApp.openById(logSpreadSheetId);
    var sheet = spr.getSheets()[0];
    var data = sheet.getDataRange().getValues();
   
    var pos = sheet.getLastRow() + 1;

    var rowData = [];
  rowData[0] = Utilities.formatDate(new Date(), timeZone, dateTimeFormat);
  rowData[4]=entrance;
  rowData[1] = uid;
  rowData[2] = name;
  rowData[3] = result;
  rowData[5] = newvar;
    var newRange = sheet.getRange(pos, 1, 1, rowData.length);
    newRange.setValues([rowData]);


}



/**
 * Remove leading and trailing single or double quotes
 */
function stripQuotes(value) {
    return value.replace(/^["']|['"]$/g, "");
}