const char root[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.jsdelivr.net/npm/@jaames/iro@5"></script>
    <title>Pwm 0</title>
    <link id="icon" rel="icon" type="image/x-icon" href="/fan.png"></link>    
    <style>
    
        body{
            margin: 0;
            padding: 0;
            display: grid;
            justify-content: center;
        }
        
        h1{
            text-align: center;
        }
          
        .range-container {
            width: 320px;
            margin: 0px auto;
            text-align: center;          
        }

        input[type="range"] {
            -webkit-appearance: none;
            appearance: none;
            width: 100%;
            height: 15px;
            border-radius: 105px;
            background: #d3d3d3;
            outline: none;
            opacity: 0.7;
            -webkit-transition: .2s;         
            transition: opacity .2s;
            border: 1px solid black;
            transition: 0.1s all ease;
        }

        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 40px;
            height: 40px;
            border-radius: 50%;
            border: 2px solid black; 
            background: #3498db;
            cursor: pointer;
            transition: 0.1s all;
            background-image: url("/fan.png");
            background-size: cover;
            transform: rotateZ(var(--thumb-rotate, 0deg));
             
        }

        input[type="range"]::-moz-range-thumb {
            appearance: none;
            width: 40px;
            height: 40px;
            border-radius: 50%;
            border: 1px solid black; 
            background: #3498db;
            cursor: pointer;
            transition: 0.1s all;
            background-image: url("/fan.png");
            background-size: cover;
            transform: rotateZ(var(--thumb-rotate, 0deg));
        }

        input[type="range"]:hover {
            opacity: 1;
            height: 25px;            
        }
        
        /* input[type="range"]:hover::-webkit-slider-thumb {
            width: 35px;
            height: 35px; 
        } */
        
    </style>
</head>
<body>

    <div>
        <h1 id="rgbCode">rgb(255, 255, 255)</h1>
        <form action='/led' method='get'>
            <div id="picker"></div>
        </form>
    </div>
    
    
    <div class="range-container">
        <form action='/move' method='get'>
            <label for="pi_input"><h1>Cooler: <output id="value">%</output></h1></label>
            <input id="pi_input" type="range" min="0" max="255" step="1" />
        </form>
    </div>
    
</body>
<script>

    const rgbTitle = document.querySelector("#rgbCode");
    const colorPicker = new iro.ColorPicker('#picker');
    colorPicker.on('color:change', function(color) {
    
      console.log(color.rgbString, color.rgb);
      rgbTitle.textContent = color.rgbString
      
    });

    const value = document.querySelector("#value");
    const input = document.querySelector("#pi_input");
    const title = document.querySelector("title");
    
    async function read(){
        try{
            const read = await fetch("http://esp8266.local/read")
                                  .then(response => response.text())
                                    //.then(result => console.log(result))
                                  .then(result =>{
                                        input.value = result;
                                        const percent = parseInt((parseInt(result) - 0)*(100 - 0)/(255 - 0))
                                        //(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
                                        title.textContent = `Pwm ${percent}%`;
                                        value.textContent = `${percent}%`;
                                    })
                                  .catch(error => console.log('error', error));  
        }
        catch(error){
            console.error('Error:', error);
        }
    }
    
    read()
    
    input.addEventListener("input", (event) => {
        value.textContent = event.target.value;
        const percent = parseInt((parseInt(event.target.value) - 0)*(100 - 0)/(255 - 0))
        //(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
        title.textContent = `Pwm ${percent}%`;
        value.textContent = `${percent}%`;
        input.style.setProperty("--thumb-rotate", `${(input.value/200) * 360}deg`);s
        sendPos();
    });
        
    const sendPos = async()=>{
        try{
            const response = await fetch(`http://esp8266.local/move?angle=${value.textContent}`);
            if (!response.ok) {
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            // const data = await response;
            // console.log(data); // Do something with the data
        }
        catch (error) {
            console.error('Error:', error);
        }
    }
    
    setInterval(async() => {
       await read();
    }, 2000);
    
</script>
</html>
)=====";
