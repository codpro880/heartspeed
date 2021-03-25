import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
import { Sunwell } from "../sunwell/MinifiedSunwell"

function get_card() {
    // TODO: Remove hardcoding in this function
    
    var img = new Image();
    let sunwell = new Sunwell({
      	// assetFolder: '../assets/',
        assetFolder: process.env.PUBLIC_URL + "/assets/",
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

    var card_name = "Mecharoo";
    var card_text = "Mecharoo text";

    sunwell.createCard({
      	//"id": "EX1_116",
      	//"dbfId": 559,
      	"name": card_name,
      	// "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
	"text": card_text,
      	// "flavor": "At least he has Angry Chicken.",
      	// "artist": "Gabe from Penny Arcade",
      	// "attack": card_json['attack'],
        "attack": 1,
      	"cardClass": "MURLOC",
      	//"collectible": true,
      	"cost": 1,
      	// "elite": true,
      	"faction": "ALLIANCE",
      	// "health": card_json['health'],
        "health": 1,
      	// "mechanics": [
      	// 	"BATTLECRY",
      	// 	"CHARGE"
      	// ],
      	"rarity": "COMMON",
      	// "set": "EXPERT1",
      	"type": "MINION",
      	// "texture": "../assets/" + card_name + ".jpg"
        "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
    }, 256, false, img, function() {
      	console.log('done');
    });

    return img;
}

class Rollout extends React.Component {  

  constructor(props) {
    console.log("Constructed.");
    super(props);
    //this.canvasRef = React.createRef();
    this.state = {
      value: get_card(),
    };
  }

  render() {

    const reducer = (_, { data }) => data;  

    const Bunny = () => {
      const [motion, update] = useReducer(reducer);
      const iter = useRef(0);
      useTick(delta => {
        const i = (iter.current += 0.05 * delta);
        update({
          type: 'update',
          data: {
            // x: 0,
            // y: 0,
            x: Math.sin(i) * 100,
            y: Math.sin(i / 1.5) * 100,
            rotation: Math.sin(i) * Math.PI,
            anchor: Math.sin(i / 2),
          },
        })
      })

      return (
        <Sprite
          image={this.state.value}
          height={90}
          width={60}
          {...motion}
        />
      )
    }
    
    return (
      <Stage width={300} height={300} options={{ transparent: true }}>
        <Container x={150} y={150}>                 
         <Bunny />
        </Container>
      </Stage>
    )
  }
}

export default Rollout;
