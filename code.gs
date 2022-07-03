// Google-Apps-Script app
// Handles the GET request and saves the received data to google sheets.

function doGet(e){
  Logger.log("--- doGet ---");
 
  try {
 
    // this helps during debuggin
    if (e == null){e={}; e.parameters = {tag:"test",value:"27.06;98548.30;26.60;45.80"};}

    let dataPayload_arr = e.parameters.value["0"].split(";");

    let dataPayload_obj = {
      BMP_Temperature: dataPayload_arr[0],
      BMP_Pressure: dataPayload_arr[1],
      DHT_Temperature: dataPayload_arr[2],
      DHT_Humidity: dataPayload_arr[3]
    }
 

    let tag = e.parameters.tag;
    let value = dataPayload_obj;
 
    // save the data to spreadsheet
    save_data(tag, value);
 
 
    return ContentService.createTextOutput("Wrote:\n  tag: " + tag + "\n  value: " + value);
 
  } catch(error) { 
    Logger.log(error);    
    return ContentService.createTextOutput("oops...." + error.message 
                                            + "\n" + new Date() 
                                            + "\ntag: " + tag +
                                            + "\nvalue: " + value);
  }  
}
 
// Method to save given data to a sheet
function save_data(tag, value){
  Logger.log("--- save_data ---"); 
 
 
  try {
    var dateTime = new Date();
 
    // Paste the URL of the Google Sheets starting from https thru /edit
    // For e.g.: https://docs.google.com/..../edit 
    let ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1_YL1ZynR3oQEMSgbeh6L2CAQVNAri5ZykgG3BgKB_hA/edit");
    let dataLoggerSheet = ss.getSheetByName("Meteostanice");
 
 
    // Get last edited row from DataLogger sheet
    let row = dataLoggerSheet.getLastRow() + 1;
 
 
    // Start Populating the data
    dataLoggerSheet.getRange("A" + row).setValue(row -1); // ID
    dataLoggerSheet.getRange("B" + row).setValue(dateTime); // dateTime
    dataLoggerSheet.getRange("C" + row).setValue(tag); // tag
    dataLoggerSheet.getRange("D" + row).setValue(value.BMP_Temperature);
    dataLoggerSheet.getRange("E" + row).setValue(value.BMP_Pressure);
    dataLoggerSheet.getRange("F" + row).setValue(value.DHT_Temperature);
    dataLoggerSheet.getRange("G" + row).setValue(value.DHT_Humidity);
 
 
    // Update summary sheet
    summarySheet.getRange("B1").setValue(dateTime); // Last modified date
    // summarySheet.getRange("B2").setValue(row - 1); // Count 
  }
 
  catch(error) {
    Logger.log(JSON.stringify(error));
  }
 
  Logger.log("--- save_data end---"); 
}