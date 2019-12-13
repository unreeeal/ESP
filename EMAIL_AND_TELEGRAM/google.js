var emailAddress = '';  // comma separate for several emails

function sendEmail(message) {
    
    MailApp.sendEmail(emailAddress, "Hi, It's your esp", message);
}
  
  
  function doGet(e) {
    var text= e.parameter.text;
    if (typeof text != 'undefined') {
      sendEmail(text);
    }
    // Return result of operation
    return ContentService.createTextOutput("ok");
  }