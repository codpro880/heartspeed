import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
import { Sunwell } from "../sunwell/MinifiedSunwell"

function getTestCardJson() {
    var json = `{
        "attack": 6,
        "has_cleave": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": false,
        "has_windfury": false,
        "health": 10,
        "name": "Glyph Guardian (Golden)"
    }`
    return JSON.parse(json);
}

function getCardText(card_json) {
    // "has_cleave": false,
    // "has_divine_shield": false,
    // "has_poison": false,
    // "has_reborn": false,
    // "has_taunt": false,
    // "has_windfury": false,
    var text = "";
    if (card_json['has_divine_shield']) {
        text += "DIVINE ";
    }
    if (card_json['has_reborn']) {
        text += "REBORN ";
    }
    if (card_json['has_taunt']) {
        text += "TAUNT ";
    }
    return text;
}


//function get_card(card_json) {
function get_card() {
    var card_json = getTestCardJson(); // TODO: Remove hardcoding in this function, should be arg
    var img = new Image();
    let asset_folder = process.env.PUBLIC_URL + "/assets/";
    let sunwell = new Sunwell({
      	// assetFolder: '../assets/',
        assetFolder: asset_folder,
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

    var card_name_raw = card_json['name'];
    var card_text = getCardText(card_json);
    var card_name = card_name_raw.replace(" (Golden)", "");

    sunwell.createCard({
      	//"id": "EX1_116",
      	//"dbfId": 559,
      	"name": card_name,
      	// "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
	// "text": card_text,
      	// "flavor": "At least he has Angry Chicken.",
      	// "artist": "Gabe from Penny Arcade",
      	// "attack": card_json['attack'],
        "attack": 1,
      	"cardClass": "MURLOC",
        "cardFrame": null,
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
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name + ".jpg"
    }, 256, false, img, function() {
      	console.log('done');
    });

    return img;
}

class Card extends React.Component {

  constructor(props) {
    super(props);
    this.state = {
      value: get_card(),
      toDraw: false,
    };

    // Are you serious, react...?
    // this.toggleAnimation = this.toggleAnimation.bind(this);
  }

  render() {
    const reducer = (_, { data }) => data;  

    const DrawCard = () => {
      const [motion, update] = useReducer(reducer);
      const iter = useRef(0);
      useTick(delta => {
        const i = (iter.current += 0.05 * delta);
        if (i > 50) {
          this.setState({toDraw: false});
        }
        if (this.state.toDraw) {
          update({
            type: 'update',
            data: {
              x: i,
              y: i,
              // x: Math.sin(i) * 100,
              // y: Math.sin(i / 1.5) * 100,
              // rotation: Math.sin(i) * Math.PI,
              // anchor: Math.sin(i / 2),
            },
          })
        }
        else {
          update({
            type: 'update',
            data: {
              x: 0,
              y: 0,
            },
          })
        }
      })

      return (
              <Sprite
                image={this.state.value}
                height={130}
                width={86}
                {...motion}
             />
      )
    }
  return (
    <DrawCard />
    )
  }
}

class Rollout extends React.Component {  

  constructor(props) {
    super(props);
    this.state = {
      value: get_card(),
      toDraw: false,
      ticker: 0,
    };

    // Are you serious, react...?
    this.toggleAnimation = this.toggleAnimation.bind(this);
  }

  toggleAnimation() {
    this.setState({toDraw: !this.state.toDraw});
  }

  render() {

    const reducer = (_, { data }) => data;  

    const Bunny = () => {
      const [motion, update] = useReducer(reducer);
      const iter = useRef(0);
      debugger;
      useTick(delta => {
        const i = (iter.current += 0.05 * delta);
        if (i > 50) {
          this.setState({toDraw: false});
        }
        if (this.state.toDraw) {
          update({
            type: 'update',
            data: {
              x: i,
              y: i,
              // x: Math.sin(i) * 100,
              // y: Math.sin(i / 1.5) * 100,
              // rotation: Math.sin(i) * Math.PI,
              // anchor: Math.sin(i / 2),
            },
          })
        }
        else {
          update({
            type: 'update',
            data: {
              x: 0,
              y: 0,
            },
          })
        }
      })

      var sprite1 = <Sprite
                       image={this.state.value}
                       height={130}
                       width={86}
                       {...motion}
                     />;
      var sprite2 = <Sprite
                       image={this.state.value}
                       height={45}
                       width={30}
                       x={50}
                       y={50}
                       {...motion}
                     />;
      var test_arr = [];
      test_arr.push(sprite1);
      test_arr.push(sprite2);
      //var sprite1 = <Sprite image={this.state.value} height={90} width=60></Sprite>;
        // <div>
        //   {sprite1}
        // </div>

        // <Sprite
        //   image={this.state.value}
        //   height={90}
        //   width={60}
        //   {...motion}
        // />

      return test_arr;

      // return (
      //   {sprite1}
      // )
    }
    
    return (
      <>
      <button onClick={this.toggleAnimation}>
        Toggle Animation
      </button>
      <Stage width={300} height={300} options={{ transparent: true }}>
        <Container x={150} y={150}>                
            <Card />
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
