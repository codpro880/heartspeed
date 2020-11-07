"use strict";


function main() {
  // Get A WebGL context
  /** @type {HTMLCanvasElement} */
  var canvas = document.querySelector("#canvas");
  var gl = canvas.getContext("webgl");
  if (!gl) {
    return;
  }

  // setup GLSL program
  var program = webglUtils.createProgramFromScripts(gl, ["drawImage-vertex-shader", "drawImage-fragment-shader"]);

  // look up where the vertex data needs to go.
  var positionLocation = gl.getAttribLocation(program, "a_position");
  var texcoordLocation = gl.getAttribLocation(program, "a_texcoord");

  // lookup uniforms
  var matrixLocation = gl.getUniformLocation(program, "u_matrix");
  var textureLocation = gl.getUniformLocation(program, "u_texture");

  // Create a buffer.
  var positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);


  // Put a unit quad in the buffer
  var positions = [
    0, 0,
    0, 1,
    1, 0,
    1, 0,
    0, 1,
    1, 1,
  ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

  // Create a buffer for texture coords
  var texcoordBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);

  // Put texcoords in the buffer
  var texcoords = [
    0, 0,
    0, 1,
    1, 0,
    1, 0,
    0, 1,
    1, 1,
  ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texcoords), gl.STATIC_DRAW);

  // creates a texture info { width: w, height: h, texture: tex }
  // The texture will start with 1x1 pixels and be updated
  // when the image has loaded
  function loadImageAndCreateTextureInfo(card_json) {
    var tex = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, tex);
    // Fill the texture with a 1x1 blue pixel.
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,
                  new Uint8Array([0, 0, 255, 255]));

    // let's assume all images are not a power of 2
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    var textureInfo = {
      width: 1,   // we don't know the size until it loads
      height: 1,
      texture: tex,
    };
    var img = new Image();
    img.addEventListener('load', function() {
      textureInfo.width = img.width;
      textureInfo.height = img.height;

      gl.bindTexture(gl.TEXTURE_2D, textureInfo.texture);
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
    });
      
      // img.src = url;
      // img.src = '/Users/matt/repos/heart_speed/src/webgui/assets/lil_rag_golden.png';

      let sunwell = new Sunwell({
      			assetFolder: '../assets/',
      			titleFont: "belwe_fsextrabold",
      			bodyFontRegular: "franklin_gothic_fsMdCn",
      			bodyFontItalic: "franklin_gothic_fsMdCnIt",
      			bodyFontBold: "franklin_gothic_fsDemiCn",
      			bodyFontBoldItalic: "franklin_gothic_fsDemiCn",
      			bodyFontSize: 38,
      			bodyLineHeight: 40,
      			bodyFontOffset: {x: 0, y: 26},
      			debug: true
      		});
      // let cardCanvas = document.getElementById('cardCanvas')
      var card_name_raw = card_json['name'];
      var card_name = card_name_raw.replace(" (Golden)", "");
      		sunwell.createCard({
      		    //"id": "EX1_116",
      		    //"dbfId": 559,
      		    "name": card_name,
      		    // "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
      		    // "flavor": "At least he has Angry Chicken.",
      		    // "artist": "Gabe from Penny Arcade",
      		    "attack": card_json['attack'],
      		    "cardClass": "MURLOC",
      		    //"collectible": true,
      		    "cost": 1,
      		    // "elite": true,
      		    "faction": "ALLIANCE",
      		    "health": card_json['health'],
      		    // "mechanics": [
      		    // 	"BATTLECRY",
      		    // 	"CHARGE"
      		    // ],
      		    "rarity": "COMMON",
      		    // "set": "EXPERT1",
      		    "type": "MINION",
      		    "texture": "../assets/" + card_name + ".jpg"
      		}, 256, false, img, function() {
      		    console.log('done')
      		});

      //img.src = 'lil_rag_golden.png'

    return textureInfo;
  }

    var textureInfos = [
	//loadImageAndCreateTextureInfo('assets/Murloc Scout.png'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/star.jpg'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/leaves.jpg'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/keyboard.jpg'),
    ];

    var drawInfos;
    var frame_num = 0;
    var frame_list = getCardFrames();

    window.addEventListener("load", function setupWebGL (evt) {
	"use strict"

	// Cleaning after ourselves. The event handler removes
	// itself, because it only needs to run once.
	window.removeEventListener(evt.type, setupWebGL, false);

	// Adding the same click event handler to both canvas and
	// button.
	var canvas = document.querySelector("#canvas");
	var forward_button = document.querySelector("#frame-forward");
	var backward_button = document.querySelector("#frame-backward");
	// canvas.addEventListener("click", switchColor, false);
	forward_button.addEventListener("click", frameForwardOnClick, false);
	backward_button.addEventListener("click", frameBackwardOnClick, false);

	// A variable to hold the WebGLRenderingContext.
	var gl;

	// The click event handler.
	function frameForwardOnClick () {
	    if (frame_num < frame_list.length) frame_num++;
	    drawInfos = populateDrawInfos(frame_num);
	}
	function frameBackwardOnClick () {
	    if (frame_num > 0) frame_num--;	    
	    drawInfos = populateDrawInfos(frame_num);
	}

    }, false);


    function populateDrawInfos(frame_num) {
	var frame_list = getCardFrames();
	var first_frame_b1 = frame_list[frame_num]["b1"]
	var first_frame_b2 = frame_list[frame_num]["b2"]
	var drawInfos = [];
	// var numToDraw = 9;
	// var numToDraw = first_frame.length;
	var numToDraw = first_frame_b1.length + first_frame_b2.length
	var speed = 60;
	gl.canvas.width = 300;
	gl.canvas.height = 150;
	
	for (var ii = 0; ii < first_frame_b1.length; ++ii) {
    	    //var percent = 1.0 / numToDraw;
    	    var percent = .5
	    var card_json = first_frame_b1[ii]
	    console.log("ii" + ii);
	    console.log("gl.canvas.width" + gl.canvas.width);
	    console.log("gl.canvas.height" + gl.canvas.height);
	    console.log("X POS: " + (percent * ii * gl.canvas.width));
	    console.log("Y POS: " + (percent * ii * gl.canvas.height));
    	    var drawInfo = {
    		// x: Math.random() * gl.canvas.width,
    		// y: Math.random() * gl.canvas.height,
    		x: percent * ii * gl.canvas.width,
    		y: 0,
    		// dx: Math.random() > 0.5 ? -1 : 1,
    		dx: Math.random() > 0.5 ? -1 : 1,
    		dy: Math.random() > 0.5 ? -1 : 1,
    		// xScale: Math.random() * 0.25 + 0.25,
    		// yScale: Math.random() * 0.25 + 0.25,
    		xScale: .5,
    		yScale: .5,
    		//textureInfo: textureInfos[Math.random() * textureInfos.length | 0],
		textureInfo: loadImageAndCreateTextureInfo(card_json),
    	    };
    	    drawInfos.push(drawInfo);
	}

	for (var jj = 0; jj < first_frame_b2.length; ++jj) {
    	    var percent = .5
	    var card_json = first_frame_b2[jj]
    	    var drawInfo = {
    		// x: Math.random() * gl.canvas.width,
    		// y: Math.random() * gl.canvas.height,
    		x: percent * jj * gl.canvas.width,
    		y: gl.canvas.height * 2,
    		// dx: Math.random() > 0.5 ? -1 : 1,
    		dx: Math.random() > 0.5 ? -1 : 1,
    		dy: Math.random() > 0.5 ? -1 : 1,
    		// xScale: Math.random() * 0.25 + 0.25,
    		// yScale: Math.random() * 0.25 + 0.25,
    		xScale: .5,
    		yScale: .5,
		//textureInfo: textureInfos[Math.random() * textureInfos.length | 0],
		
    		textureInfo: loadImageAndCreateTextureInfo(card_json),
    	    };
    	    drawInfos.push(drawInfo);
	}
	return drawInfos;
    }
    drawInfos = populateDrawInfos(0);
    var update_now = true;
    var dinfo_to_update = drawInfos[0];
    var dinfo_dest = [50, 50];
    var num_frames = 50;

    function update(deltaTime) {
	if (update_now) {
	    dinfo_to_update.x = dinfo_to_update.x + 1;
	    dinfo_to_update.y = dinfo_to_update.y + 1;
	    if (dinfo_to_update.x === 50) update_now = false;
	}
	
	// drawInfos.forEach(function(drawInfo) {
	//     // drawInfo.x += drawInfo.dx * speed * deltaTime;
	//     // drawInfo.y += drawInfo.dy * speed * deltaTime;
	//     if (drawInfo.x < 0) {
	// 	drawInfo.dx = 1;
	//     }
	//     if (drawInfo.x >= gl.canvas.width) {
	// 	drawInfo.dx = -1;
	//     }
	//     if (drawInfo.y < 0) {
	// 	drawInfo.dy = 1;
	//     }
	//     if (drawInfo.y >= gl.canvas.height) {
	// 	drawInfo.dy = -1;
	//     }
	// });
    }

  function draw() {
    webglUtils.resizeCanvasToDisplaySize(gl.canvas);

    // Tell WebGL how to convert from clip space to pixels
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    gl.clear(gl.COLOR_BUFFER_BIT);

      drawInfos.forEach(function(drawInfo) {
	  // console.log("X POS: " + drawInfo.x);
	  // console.log("Y POS: " + drawInfo.y);
      var dstX      = drawInfo.x;
      var dstY      = drawInfo.y;
      var dstWidth  = drawInfo.textureInfo.width  * drawInfo.xScale;
      var dstHeight = drawInfo.textureInfo.height * drawInfo.yScale;

      drawImage(
        drawInfo.textureInfo.texture,
        drawInfo.textureInfo.width,
        drawInfo.textureInfo.height,
        dstX, dstY, dstWidth, dstHeight);
    });
  }

  var then = 0;
  function render(time) {
    var now = time * 0.001;
    var deltaTime = Math.min(0.1, now - then);
    then = now;

    update(deltaTime);
    draw();

    requestAnimationFrame(render);
  }
  requestAnimationFrame(render);

  // Unlike images, textures do not have a width and height associated
  // with them so we'll pass in the width and height of the texture
  function drawImage(
      tex, texWidth, texHeight,
      dstX, dstY, dstWidth, dstHeight) {
    if (dstWidth === undefined) {
      dstWidth = texWidth;
    }

    if (dstHeight === undefined) {
      dstHeight = texHeight;
    }

    gl.bindTexture(gl.TEXTURE_2D, tex);

    // Tell WebGL to use our shader program pair
    gl.useProgram(program);

    // Setup the attributes to pull data from our buffers
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    gl.enableVertexAttribArray(positionLocation);
    gl.vertexAttribPointer(positionLocation, 2, gl.FLOAT, false, 0, 0);
    gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);
    gl.enableVertexAttribArray(texcoordLocation);
    gl.vertexAttribPointer(texcoordLocation, 2, gl.FLOAT, false, 0, 0);

    // this matrix will convert from pixels to clip space
    var matrix = m4.orthographic(0, gl.canvas.width, gl.canvas.height, 0, -1, 1);

    // this matrix will translate our quad to dstX, dstY
    matrix = m4.translate(matrix, dstX, dstY, 0);

    // this matrix will scale our 1 unit quad
    // from 1 unit to texWidth, texHeight units
    matrix = m4.scale(matrix, dstWidth, dstHeight, 1);

    // Set the matrix.
    gl.uniformMatrix4fv(matrixLocation, false, matrix);

    // Tell the shader to get the texture from texture unit 0
    gl.uniform1i(textureLocation, 0);

    // draw the quad (2 triangles, 6 vertices)
    gl.drawArrays(gl.TRIANGLES, 0, 6);
  }

    function getCardFrames() {
	var json = `[
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            },
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            },
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            }
        ],
        "b1_turn": 1,
        "b2": [
            {
                "attack": 6,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 6,
                "name": "Freedealing Gambler (Golden)"
            },
            {
                "attack": 6,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 6,
                "name": "Freedealing Gambler (Golden)"
            }
        ],
        "defender_pos": 0
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            },
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            }
        ],
        "b1_turn": 0,
        "b2": [
            {
                "attack": 6,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": false,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 6,
                "name": "Freedealing Gambler (Golden)"
            }
        ],
        "defender_pos": 1
    },
    {
        "attacker_pos": 0,
        "b1": [
            {
                "attack": 999999,
                "has_cleave": false,
                "has_divine_shield": false,
                "has_poison": true,
                "has_reborn": false,
                "has_taunt": false,
                "has_windfury": false,
                "health": 2,
                "name": "Murloc Tidehunter (Golden)"
            }
        ],
        "b1_turn": 0,
        "defender_pos": 0
    }
]`;
	return JSON.parse(json);
    }

}

main();
