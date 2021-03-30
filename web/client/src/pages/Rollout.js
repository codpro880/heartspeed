import React, { useReducer, useRef } from 'react';

import { Container, Stage, Sprite, useTick, } from '@inlet/react-pixi';
import { Sunwell } from "../sunwell/MinifiedSunwell"
 
function getTestCardJson(attack, health, name) {
    var json = {
        "attack": attack,
        "has_cleave": false,
        "has_divine_shield": false,
        "has_poison": false,
        "has_reborn": true,
        "has_taunt": false,
        "has_windfury": false,
        "health": health,
        "name": name
    }
    return json;
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

function get_card(card_json) {    
    var img = new Image();
    let asset_folder = process.env.PUBLIC_URL + "/assets/";
    let sunwell = new Sunwell({
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

    var is_golden = card_name_raw !== card_name;

    sunwell.createCard({
      	//"id": "EX1_116",
      	//"dbfId": 559,
      	"name": card_name,
      	// "text": "<b>Charge</b>. <b>Battlecry:</b> Summon two 1/1 Whelps for your opponent.",
	// "text": card_text,
      	// "flavor": "At least he has Angry Chicken.",
      	// "artist": "Gabe from Penny Arcade",
      	"attack": card_json['attack'],
      	"cardClass": "MURLOC",
        "cardFrame": null,
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
      	// "texture": "../assets/" + card_name + ".jpg"
        // "texture": process.env.PUBLIC_URL + "/assets/Mecharoo.png"
        "texture": asset_folder + card_name + ".jpg"
    }, 256, is_golden, img, function() {
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
              x: this.props.startX + i,
              y: this.props.startY + i,
            },
          })
        }
        else {
          update({
            type: 'update',
            data: {
              x: this.props.startX,
              y: this.props.startY,
            },
          })
        }
      })

      return (
              <Sprite
                image={this.props.img}
                height={70}
                width={46}
                {...motion}
             />
      )
    }
  return (
    <DrawCard />
    )
  }
}

const BOARD_WIDTH = 600;
const BOARD_HEIGHT = 350;
const BOARD_WIDTH_FUDGE = -BOARD_WIDTH / 20.0; // -30
const TOP_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 14; // -25
const BOTTOM_BOARD_HEIGHT_FUDGE = -BOARD_HEIGHT / 5; // -70

class Rollout extends React.Component {

  constructor(props) {
    super(props);
    this.state = {
      toDraw: false,
      backgroundImage: process.env.PUBLIC_URL + "/assets/hearthstone_battle_phase_background.jpeg",
    };

    // Are you serious, react...?
    this.toggleAnimation = this.toggleAnimation.bind(this);
    this.createCards = this.createCards.bind(this);
  }

  toggleAnimation() {
    this.setState({toDraw: !this.state.toDraw});
  }

  createCards(frame) {
    var card1_json = getTestCardJson(10, 10, "Mecharoo");
    var card1 = <Card key={"card1"}
                  toDraw={this.state.toDraw}
                  img={get_card(card1_json)}
                  startX={BOARD_WIDTH / 2.0 + BOARD_WIDTH_FUDGE}
                  startY={BOARD_HEIGHT / 3.0 + TOP_BOARD_HEIGHT_FUDGE}
                >
                </Card>;
    // TODO: Debug the script that ropes in these assets
    var card2_json = getTestCardJson(20, 20, "Razorgor (Golden)");
    var card2 = <Card key={"card2"}
                  toDraw={this.state.toDraw}
                  img={get_card(card2_json)}               
                  startX={BOARD_WIDTH / 2.0 + BOARD_WIDTH_FUDGE}
                  startY={2 * BOARD_HEIGHT / 3.0 + BOTTOM_BOARD_HEIGHT_FUDGE}
                >
                </Card>;
    var card_arr = [];
    card_arr.push(card1);
    card_arr.push(card2);
    return card_arr;
  }

  render() {
    const card_arr = this.createCards(null);
    
    return (
      <>
      <button onClick={this.toggleAnimation}>
          Toggle Animation
      </button>
      <Stage width={BOARD_WIDTH} height={BOARD_HEIGHT} options={{ transparent: true }}>
        <Container x={0} y={0}>
            <Sprite
                image={this.state.backgroundImage}
                width={BOARD_WIDTH}
                height={BOARD_HEIGHT}
             />
            {card_arr}
        </Container>
      </Stage>
      </>
    )
  }
}

export default Rollout;
