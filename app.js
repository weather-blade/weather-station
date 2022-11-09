// Google-Apps-Script web app script
// Handles the POST request and saves the received data to google sheets.

function doPost(e) {
  console.log("Recieved POST request");
  try {
    const body = e === undefined ?
      // Debug values - only when running from console
      {
        "tag": "debugging",
        "BMP_Temperature": 11.11,
        "BMP_Pressure": 11.11,
        "DHT_Temperature": 11.11,
        "DHT_Humidity": 11.11
      }
      :
      JSON.parse(e.postData.contents) // parse body of the POST request 

    console.log("POST request body: \n", body);

    saveData(body);

    console.log("POST request handled succesfully");
  }

  catch (error) {
    console.error("Processing POST request failed: \n", error);
  }
}

// saves given data to sheet
function saveData(data) {
  console.log("Saving data to spreadsheet");

  const maxTries = 3;
  let triesCount = 0;

  while (true) {
    try {
      const dateTime = new Date();
      const values = [[dateTime, data.tag, data.BMP_Temperature, data.BMP_Pressure, data.DHT_Temperature, data.DHT_Humidity]];
      console.log("Saving values to spreadsheets: ", values);

      const ss = SpreadsheetApp.getActiveSpreadsheet();
      console.log("Selected active spreadsheet: ", ss.getName());

      const dataLoggerSheet = ss.getSheetByName("Meteostanice");
      console.log("Selected data logger sheet: ", dataLoggerSheet.getName());

      // Makes new row just bellow the header to insert new data into
      dataLoggerSheet.insertRowAfter(1);
      console.log("Inserted new row at the top");

      const range = dataLoggerSheet.getRange(2, 2, 1, 6); // 2nd row, 2nd column, select 1 row and 6 columns
      console.log("Selected range: ", range.getA1Notation());

      range.setValues(values);
      console.log("Saved values: ", range.getValues());

      console.log("Data saved succesfully");
      return;
    }

    catch (error) {
      console.warn("Saving data failed: \n", error);

      // getting access sometimes fails
      if (++triesCount === maxTries) {
        throw "Could not save data to spreadsheet";
      }

      console.warn("Retrying to save data");
    }
  }
}