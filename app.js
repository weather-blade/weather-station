// Google-Apps-Script web app script
// Handles the POST request and saves the received data to google sheets.

function doPost(e) {
  console.log("Recieved POST request");
  try {
    // DEBUG
    //const body = {
    //  "tag": "debugging",
    //  "BMP_Temperature": 11,
    //  "BMP_Pressure": 11,
    //  "DHT_Temperature": 11,
    //  "DHT_Humidity": 11
    //}

    const body = JSON.parse(e.postData.contents);

    console.log("POST request body: \n", body);

    saveData(body);

    console.log("POST request handled succesfully");
  }

  catch (error) {
    console.log("Processing POST request failed: \n", error);
  }
}

// saves given data to sheet
function saveData(data) {
  console.log("Saving data to spreadsheet");
  try {
    const dateTime = new Date();

    const ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1_YL1ZynR3oQEMSgbeh6L2CAQVNAri5ZykgG3BgKB_hA/edit");
    const dataLoggerSheet = ss.getSheetByName("Meteostanice");

    // Makes new row just bellow the header to insert new data into
    dataLoggerSheet.insertRowAfter(1);

    // For inserting into the newly created row
    const row = 2;

    // Start Populating the data
    dataLoggerSheet.getRange("B" + row).setValue(dateTime);
    dataLoggerSheet.getRange("C" + row).setValue(data.tag);
    dataLoggerSheet.getRange("D" + row).setValue(data.BMP_Temperature);
    dataLoggerSheet.getRange("E" + row).setValue(data.BMP_Pressure);
    dataLoggerSheet.getRange("F" + row).setValue(data.DHT_Temperature);
    dataLoggerSheet.getRange("G" + row).setValue(data.DHT_Humidity);

    console.log("Data saved succesfully");
  }

  catch (error) {
    console.log("Saving data failed: \n", error);
  }
}