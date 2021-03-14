var appRouter = function(app) {
    app.get("/", function(req, res) {
        res.send("Hello World");
    });
    app.get("/echo", function(req, res) {
        console.log(req.body);
        res.send(req.body);
    });
    app.post("/", function(req, res) {
        console.log(req.body);
        res.send(req.body);
    });
}

module.exports = appRouter;