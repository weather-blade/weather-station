// Google-Apps-Script app
// Handles the GET request and saves the received data to google sheets.

function doGet(e){
  Logger.log("--- doGet ---");

  try {
    // for debugging
    if (e == null){e={}; e.parameters = {tag:"debug",value:"21.21;22.22;23.23;24.24"};}

    // splits the parameters
    const dataPayload_arr = e.parameters.value["0"].split(";");
    const dataPayload_obj = {
      BMP_Temperature: dataPayload_arr[0],
      BMP_Pressure: dataPayload_arr[1],
      DHT_Temperature: dataPayload_arr[2],
      DHT_Humidity: dataPayload_arr[3]
    }

    // save the data to spreadsheet
    const tag = e.parameters.tag;
    const value = dataPayload_obj;
    save_data(tag, value);

    return ContentService.createTextOutput("Wrote:\n  tag: " + tag + "\n  value: " + value);
  }
  
  catch(error) { 
    Logger.log(error);    
    return ContentService.createTextOutput("oops...." + error.message 
                                            + "\n" + new Date() 
                                            + "\ntag: " + tag +
                                            + "\nvalue: " + value);
  }  
}
 
// saves given data to sheet
function save_data(tag, value){
  Logger.log("--- save_data ---"); 
 
  try {
    const dateTime = new Date();
 
    // Paste the URL of the Google Sheet starting from https thru /edit
    // For e.g.: https://docs.google.com/..../edit 
    const ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1_YL1ZynR3oQEMSgbeh6L2CAQVNAri5ZykgG3BgKB_hA/edit");
    const dataLoggerSheet = ss.getSheetByName("Meteostanice");

    // Makes new row just bellow the header to insert new data into
    dataLoggerSheet.insertRowAfter(1);

    // For inserting into the newly created row
    const row = 2;
  
    // Start Populating the data
    dataLoggerSheet.getRange("B" + row).setValue(dateTime); // dateTime
    dataLoggerSheet.getRange("C" + row).setValue(tag); // tag
    dataLoggerSheet.getRange("D" + row).setValue(value.BMP_Temperature);
    dataLoggerSheet.getRange("E" + row).setValue(value.BMP_Pressure);
    dataLoggerSheet.getRange("F" + row).setValue(value.DHT_Temperature);
    dataLoggerSheet.getRange("G" + row).setValue(value.DHT_Humidity);
  }
 
  catch(error) {
    Logger.log(JSON.stringify(error));
  }
 
  Logger.log("--- save_data end---"); 
}
