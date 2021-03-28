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

  // Would be nice if there was an easy way to define required props
  // Looks like it's sort of possible w/ PropTypes but seems like too much for now
  constructor(props) {
    super(props);
  }

  render() {
    const reducer = (_, { data }) => data;  

    const DrawCard = () => {
      const [motion, update] = useReducer(reducer);
      const iter = useRef(0);
      useTick(delta => {
        const i = (iter.current += 0.05 * delta);
        if (i > 20) {
          // I have the feeling this is wasteful
          // TODO: Figure out how to stop calling the useTick hook
          //       after animation ends
          return;
        }
        if (this.props.toDraw) {
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
                image={this.props.img}
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
      toDraw: false,
    };

    // Are you serious, react...?
    this.toggleAnimation = this.toggleAnimation.bind(this);
  }

  toggleAnimation() {
    this.setState({toDraw: !this.state.toDraw});
  }

  render() {

    var card = <Card key={"card1"} toDraw={this.state.toDraw} img={get_card()}></Card>;
    var card_arr = [];
    card_arr.push(card);
    
    return (
      <>
      <button onClick={this.toggleAnimation}>
        Toggle Animation
      </button>
      <Stage width={300} height={300} options={{ transparent: true }}>
        <Container x={150} y={150}>      
            {card_arr}
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
