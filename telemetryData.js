const fs = require('fs');
module.exports = {
    addEnvReport: function (reportJson) {
        var environmentDataJson = fs.readFileSync("./environmentData.json");
        var environmentData = JSON.parse(environmentDataJson);
        var newReport = JSON.parse(reportJson);
        var myTime = new Date();
        // var newReportJson = '{"T":' + newReport.T
        //     + ',"H":' + newReport.H
        //     + ', "time": "' + (myTime.getMonth() + 1) + "/" + (myTime.getDay() + 1) + "/" + myTime.getFullYear() + " " + myTime.toLocaleTimeString("en-US", {timeZone: "America/New_York"}) + '"'
        //     + ', "timestamp": "' + Date.now() + '"' 
        //     + '}';
        var newReportJson1 = new Object();
        newReportJson1.T = newReport.T;              
        newReportJson1.H = newReport.H;              
        newReportJson1.time = (myTime.getMonth() + 1) + "/" + (myTime.getDay() + 1) + "/" + myTime.getFullYear() + " " + myTime.toLocaleTimeString("en-US", {timeZone: "America/New_York"});              
        newReportJson1.timestamp = Date.now();              
        environmentData.data.push(newReportJson1);
        // environmentData.data.push(JSON.parse(newReportJson));
        fs.writeFile("./environmentData.json", JSON.stringify(environmentData), "utf8");
        return JSON.stringify(newReportJson1);
        // return newReportJson;
    }
}