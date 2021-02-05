const fs = require('fs');
module.exports = {
    addEnvReport: function (reportJson) {
        // grab existing storasge file and read in contents
        var environmentDataJson = fs.readFileSync("./environmentData.json");
        var environmentData = JSON.parse(environmentDataJson);
        // convert incoming report string to json
        var newReport = JSON.parse(reportJson);

        var myTime = new Date();

        // first way to build incoming report object, requires additional parsing code
        var newReportJson = `{"T":${newReport.T},
        "H":${newReport.T},
        "time":"${(myTime.getMonth() + 1)}/${(myTime.getDay() + 1)}/${myTime.getFullYear()} ${myTime.toLocaleTimeString("en-US", { timeZone: "America/New_York" })}", 
        "timestamp":${Date.now()}}`
        //
        // second way to build incoming object, requires additional parsing code
        var newReportJson2 = '{"T":' + newReport.T
            + ',"H":' + newReport.H
            + ', "time": "' + (myTime.getMonth() + 1) + "/" + (myTime.getDay() + 1) + "/" + myTime.getFullYear() + " " + myTime.toLocaleTimeString("en-US", { timeZone: "America/New_York" }) + '"'
            + ', "timestamp": "' + Date.now() + '"'
            + '}';
        //
        // third way to build incoming object, ready for use without parsing     
        var newReportJson1 = new Object();
        newReportJson1.T = newReport.T;
        newReportJson1.H = newReport.H;
        newReportJson1.time = (myTime.getMonth() + 1) + "/" + (myTime.getDay() + 1) + "/" + myTime.getFullYear() + " " + myTime.toLocaleTimeString("en-US", { timeZone: "America/New_York" });
        newReportJson1.timestamp = Date.now();
        //

        environmentData.data.push(newReportJson1);
        // environmentData.data.push(JSON.parse(newReportJson));
        fs.writeFile("./environmentData.json", JSON.stringify(environmentData), "utf8");
        return JSON.stringify(newReportJson1);
    }
}