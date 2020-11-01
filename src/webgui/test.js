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
  function loadImageAndCreateTextureInfo(url) {
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
      		sunwell.createCard({
      			//"id": "EX1_116",
      			"dbfId": 559,
      			"name": "Leeroy Jenkins",
      			"text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
      			"flavor": "At least he has Angry Chicken.",
      			"artist": "Gabe from Penny Arcade",
      			"attack": 10,
      			"cardClass": "NEUTRAL",
      			"collectible": true,
      			"cost": 5,
      			"elite": true,
      			"faction": "ALLIANCE",
      			"health": 3,
      			"mechanics": [
      				"BATTLECRY",
      				"CHARGE"
      			],
      			"rarity": "LEGENDARY",
      			"set": "EXPERT1",
      			"type": "MINION",
      			"texture": "../textures/EX1_116.jpg"
      		}, 256, false, img, function() {
      			console.log('done')
      		});

      //img.src = 'lil_rag_golden.png'

    return textureInfo;
  }

    var textureInfos = [
	loadImageAndCreateTextureInfo('lil_rag_golden.png'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/star.jpg'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/leaves.jpg'),
    //loadImageAndCreateTextureInfo('https://webglfundamentals.org/webgl/resources/keyboard.jpg'),
  ];

  var drawInfos = [];
    // var numToDraw = 9;
    var numToDraw = 9;
  var speed = 60;
    for (var ii = 0; ii < numToDraw; ++ii) {
	//var percent = 1.0 / numToDraw;
	var first_row = ii < numToDraw / 2;
	var percent = .5
	var drawInfo = {
	    // x: Math.random() * gl.canvas.width,
	    // y: Math.random() * gl.canvas.height,
	    x: first_row ? percent * ii * gl.canvas.width : percent * (ii - numToDraw/2) * gl.canvas.width,
	    y: first_row ? 0 : gl.canvas.height * 2,
	    // dx: Math.random() > 0.5 ? -1 : 1,
	    dx: Math.random() > 0.5 ? -1 : 1,
	    dy: Math.random() > 0.5 ? -1 : 1,
	    // xScale: Math.random() * 0.25 + 0.25,
	    // yScale: Math.random() * 0.25 + 0.25,
	    xScale: .5,
	    yScale: .5,
	    textureInfo: textureInfos[Math.random() * textureInfos.length | 0],
	};
	drawInfos.push(drawInfo);
  }

  function update(deltaTime) {
    drawInfos.forEach(function(drawInfo) {
      // drawInfo.x += drawInfo.dx * speed * deltaTime;
      // drawInfo.y += drawInfo.dy * speed * deltaTime;
      if (drawInfo.x < 0) {
        drawInfo.dx = 1;
      }
      if (drawInfo.x >= gl.canvas.width) {
        drawInfo.dx = -1;
      }
      if (drawInfo.y < 0) {
        drawInfo.dy = 1;
      }
      if (drawInfo.y >= gl.canvas.height) {
        drawInfo.dy = -1;
      }
    });
  }

  function draw() {
    webglUtils.resizeCanvasToDisplaySize(gl.canvas);

    // Tell WebGL how to convert from clip space to pixels
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    gl.clear(gl.COLOR_BUFFER_BIT);

    drawInfos.forEach(function(drawInfo) {
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

}
main();