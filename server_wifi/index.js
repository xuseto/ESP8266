/**
 *  Proyector que lee la temperatura del sensor del C200
 * 
 *    -------    ---------           -----             --------
 *   | C2000 |->| ESP8266 | ==//==> | Rpi |  <==//==> | PC WEB |
 *    -------    ---------           -----             --------
 */
/**
 *  Conectar con la RPi por SSH
 * ssh -X pi@192.168.1.200
 * 
 *  Transferir archivos a la RPi:
 * scp /home/xuseto/Proyectos/VisualStudio/Temp_C2000/*.*    pi@192.168.1.200:/home/pi/Proyectos/TempC2000
 * scp /home/xuseto/Proyectos/VisualStudio/Temp_C2000/public/*.*    pi@192.168.1.200:/home/pi/Proyectos/TempC2000/public/
 */

 
/*---------------------------------------------------------------------------------------------------------------------------
                    ╦═╗  ╔═╗  ╔═╗   ╦ ╦  ╦  ╦═╗  ╔═╗
                    ╠╦╝  ║╣   ║═╬╗  ║ ║  ║  ╠╦╝  ║╣ 
                    ╩╚═  ╚═╝  ╚═╝╚  ╚═╝  ╩  ╩╚═  ╚═╝
---------------------------------------------------------------------------------------------------------------------------*/
 // Express es una infraestructura de aplicaciones web Node.js mínima y flexible 
 const  express     = require('express');
 const  app         = express(); 

 // HTTP es para crear el servidor Web HTTP
 var    server     = require('http').Server(app);
 // Socket.IO permite la comunicación en tiempo real, bidireccional y basada en eventos entre el navegador y el servidor
 var    io         = require('socket.io')(server);

 // Manejo de los GPIOs de las RPi
 var    Gpio       = require('onoff').Gpio;
 // Para ejecutar las funciones en el terminal de Linux
 const  exec       = require('child_process').exec;
 // Para crear eventos al leer la temperatura de la RPi
 const  Event      = require('events');
 // Librerias para conectarse con un cliente TCP/IP 
 var    net        = require('net');

/*---------------------------------------------------------------------------------------------------------------------------
                    ╦  ╦  ╔═╗  ╦═╗
                    ╚╗╔╝  ╠═╣  ╠╦╝
                     ╚╝   ╩ ╩  ╩╚═
---------------------------------------------------------------------------------------------------------------------------*/
// Var que fuerza a ejecutar una funcion cada 1sg
var LeerTempRPi = setInterval(fc_LeerTempRPi, 1000);
// Var donde almacena la tempertaruta de la RPi
var TempRPi;
// Var donde almacena la tempertaruta del microcontrolador C2000
var TempC2000;
// Var para enviar por socket los datos que leera el cliente
var sockets={};
// Para crear eventos al leer la temperatura de la RPi
var EventUpdateTemp  = new Event.EventEmitter();
// Creamos el cliente 
const C2000     = new   net.Socket();
const ESP8266   = new   net.Socket();


/*---------------------------------------------------------------------------------------------------------------------------
                        ╔═╗  ╔═╗  ╦  ╔═╗
                        ║ ╦  ╠═╝  ║  ║ ║
                        ╚═╝  ╩    ╩  ╚═╝
---------------------------------------------------------------------------------------------------------------------------*/
// Configurar GPIO pin 21 como salida digital
const LEDRojo = new Gpio(21, 'out'); 


/*---------------------------------------------------------------------------------------------------------------------------
                        ╔═╗  ╔═╗  ╔═╗    ╦ ╦  ╔═╗  ╔╗ 
                        ╠═╝  ╠═╣  ║ ╦    ║║║  ║╣   ╠╩╗
                        ╩    ╩ ╩  ╚═╝    ╚╩╝  ╚═╝  ╚═╝
---------------------------------------------------------------------------------------------------------------------------*/
// Indicamos donde esta la pagina Web
app.use(express.static(__dirname + '/public/'));
// En caso de error muestra error 404
app.get ('*', function (req, res) {
    res.status(404).send("Error 404 - Page not found");
});
// Configramos el puerto
server.listen(3000, ()=>{
    console.log ( 'Express se inicia en http://192.168.1.200:3000 Pulse Ctrl-C para terminar ...' );
});
//
// Enviamos por socket al cliente los datos
//
io.on('connection',function (socket){
    console.log("Alguien se ha conectado por sockets");

    sockets[socket.id]=function(data)
    {
        //Almacenar callback en objeto sockets
        console.log(socket.id,TempRPi,TempC2000);
        socket.emit('data',{TempRPi, TempC2000});
    };
    EventUpdateTemp.on('NewTemperatura', sockets[socket.id]);

    C2000.on ('data', function(data)
    {
        // console.log (data);
        TempC2000 =  parseFloat(data)/10;
        console.log(data);
    });

    ESP8266.on ('data', function(data)
    {
        console.log(data);
    });

    socket.on('message', function(data) { 
        console.log(data);
      });

    // Si se desconecta algún socket lo eliminamos
    socket.on('disconnect', function (){
        console.log("Este socket se ha desconectado : ", socket.id);
        EventUpdateTemp.removeListener('NewTemperatura',sockets[socket.id]); 
        delete sockets[socket.id]; //borrar de la tabla el id+callback 
    });

    // Si se recibe datos
    socket.on('data', function (data){
        console.log(data, socket.id);
        
    });
}); // FIN io.on


/*---------------------------------------------------------------------------------------------------------------------------
                        ╔═╗  ╔═╗  ╦  ╔═╗
                        ║ ╦  ╠═╝  ║  ║ ║
                        ╚═╝  ╩    ╩  ╚═╝
---------------------------------------------------------------------------------------------------------------------------*/
// Funcion que se ejectua cada 1sg. Donde hace Toggle del LEDRojo y lee temperatura de la RPi
function fc_LeerTempRPi()
{
    if (LEDRojo.readSync() === 0) { //check the pin state, if the state is 0 (or off)
        LEDRojo.writeSync(1); //set pin state to 1 (turn LED on)
      } 
    else {
        LEDRojo.writeSync(0); //set pin state to 0 (turn LED off)
      }
    // Enviamos la temperatura de la RPi cada 1 sg
    exec("cat /sys/class/thermal/thermal_zone0/subsystem/thermal_zone0/temp", 
        function (error, stdout, stderr) 
        {   
            if (error !== null) 
            {
                console.log('exec error: ' + error);
            } else 
            {              
              TempRPi = parseFloat(stdout)/1000; 
              // Comprobamos que haya clientes conectados a nuestro servidor
              if  (io.engine.clientsCount  > 0)
                // Generamos un evento a NewTemperatura
                EventUpdateTemp.emit ('NewTemperatura',TempRPi);       
            }
    });// FIN exec
} // FIN LeerTempRPi

/*---------------------------------------------------------------------------------------------------------------------------
                        ╔╦╗╔═╗╔═╗╦╔═╗  ╔═╗╔═╗╔═╗
                         ║ ║  ╠═╝║╠═╝  ║╣ ╚═╗╠═╝
                         ╩ ╚═╝╩  ╩╩    ╚═╝╚═╝╩  
---------------------------------------------------------------------------------------------------------------------------*/
//const  ESP8266app  = express(); 
var    bodyParser = require('body-parser') ;

const app2 = express();
app2.use (bodyParser.urlencoded({ extended: true }));

app2.post('*', (req, res) => {
    //console.log('Got body:', req.body);
    console.log(res.size);
    console.log('Got body:', req.body);

    res.sendStatus(200);
});

app2.listen(3001, () => console.log(`Started server at http://localhost:3001!`));
    // var intervalConnect_esp8266 = false;

// //Conectamos con el cliente
// function connectESP8266 ()
// {
//     ESP8266.connect 
//     ({
//         port: 5000, 
//         host: '192.168.1.199'
//     });  
// }

// // Funcion que reconectar con el cliente cada 5sg
// function ReconexionCliente() {
//     if(false != intervalConnect_esp8266) 
//         return;
//         intervalConnect_esp8266 = setInterval(connectESP8266, 5000);
// }
// // Funcion que destruye la reconexion con el cliente
// function DestruirReconexionCliente() {
//     if(false == intervalConnect_esp8266) 
//         return;
//     clearInterval(intervalConnect_esp8266);
//     intervalConnect_esp8266 = false;
// }


// ESP8266.on('connect', function() 
// {
//     console.log ("Conectado ESP8266");
//     //ESP8266.write ("HOLA ESP8266 \n");
//     // Una vez conectado con el cliente destruimos la funcion de reconectar
//     DestruirReconexionCliente ();
// });// FIN ESP8266.connect

// ESP8266.on('error', function(err) 
// {
//     //console.log(err)
//     // En caso de no estar listo el cliente lo volvemos a intenar
//     ReconexionCliente();
// });

// // Esta funcion se lleva a io.on
// // ESP8266.on ('data', function(data)
// // {
// //     console.log (data);
// //     //ESP8266.destroy();
// // });

// ESP8266.on("close", function() 
// {
//     console.log("Connection closed");
//     connectESP8266();
// })

// ESP8266.on("end", function()  
// {
//     console.log("Connection ended");
//     connectESP8266();
// })

// // Llamar a la funcion para conectarse con el cliente
// connectESP8266 ();

//---------------------------------------------------------------------------------------------------------------------------
//     Cliente TCP/IP : C2000
//---------------------------------------------------------------------------------------------------------------------------
//var intervalConnect = false;

// //Conectamos con el cliente
// function connectC2000 ()
// {
//     C2000.connect 
//     ({
//         port: 3001, 
//         host: '192.168.1.106'
//     });  //'192.168.1.199'
// }

// // Funcion que reconectar con el cliente cada 5sg
// function ReconexionCliente() {
//     if(false != intervalConnect) 
//         return;
//     intervalConnect = setInterval(connectC2000, 5000);
// }
// // Funcion que destruye la reconexion con el cliente
// function DestruirReconexionCliente() {
//     if(false == intervalConnect) 
//         return;
//     clearInterval(intervalConnect);
//     intervalConnect = false;
// }


// C2000.on('connect', function() 
// {
//     //console.log ("Conectado C2000");
//     C2000.write ("HOLA C2000 \n");
//     // Una vez conectado con el cliente destruimos la funcion de reconectar
//     DestruirReconexionCliente ();
// });// FIN C2000.connect

// C2000.on('error', function(err) 
// {
//     //console.log(err)
//     // En caso de no estar listo el cliente lo volvemos a intenar
//     ReconexionCliente();
// });

// // Esta funcion se lleva a io.on
// // C2000.on ('data', function(data)
// // {
// //     console.log (data);
// //     //C2000.destroy();
// // });

// C2000.on("close", function() 
// {
//     console.log("Connection closed");
//     connectC2000();
// })

// C2000.on("end", function()  
// {
//     console.log("Connection ended");
//     connectC2000();
// })

// // Llamar a la funcion para conectarse con el cliente
// connectC2000 ();