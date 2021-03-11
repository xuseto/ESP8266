
var socket = io.connect(location.host, { 'forceNew': true });
var datax=[]
var temperatura2Array=[]
var temperaturaArray=[]

var ctx = document.getElementById("myChart").getContext('2d');
var chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: datax,
        datasets: [{
            label: "Temp. RPi",
            data: temperaturaArray,
            borderWidth: 1,
            cubicInterpolationMode: 'default',
            fill: false,
            //lineTension: 0, //for linear interpolate
            borderColor: 'rgba(255, 0, 0, 1)',
            showLine: true
        },
        {
            label: "Temp. C200",
            data: temperatura2Array,
            borderWidth: 1,
            cubicInterpolationMode: 'default',
            fill: false,
            //lineTension: 0, //for linear interpolate
            borderColor: 'rgba(0, 0,255, 1)',
            showLine: true
        }]
    },
    options: {
        hover: {
            animationDuration: 0, // duration of animations when hovering an item
        },
        responsiveAnimationDuration: 0, // animation duration after a resize
        animation: {
            duration: 0, // general animation time
        },
        responsive: true,
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true
                }
            }]
        }
    }
});

socket.on('data',(data)=>{
    //console.log("Recibimos datos del servidor");

    var dateVar = new Date();
    temperaturaArray.push(data.TempRPi);
    temperatura2Array.push(data.TempC2000);
    datax.push(dateVar.getHours() + ':' + dateVar.getMinutes() + ':' + dateVar.getSeconds() );

    if (datax.length > 25) {
        temperatura2Array.shift();
        temperaturaArray.shift();
        datax.shift();
    }


    chart.update();
})


