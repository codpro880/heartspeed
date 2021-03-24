const Sunwell=function(){"use strict";function t(t,e){function s(){this.constructor=t}for(var r in e)e.hasOwnProperty(r)&&(t[r]=e[r]);t.prototype=null===e?Object.create(e):(s.prototype=e.prototype,new s)}function e(t,e){return"string"==typeof t?t in e?e[t]:e.INVALID:t||0}function s(t){for(var e=t.canvas.width,s=t.canvas.height,r=t.getImageData(0,0,e,s).data,i=999,a=999,o=0,n=0,h=!1,d=s-1;d>-1&&!h;d--)for(var l=0;l<e;l++)if(r[d*(4*e)+4*l+3]>0){n=Math.max(n,d),h=!0;break}if(void 0===n)return null;t:for(l=e-1;l>-1;l--)for(d=0;d<s;d++)if(r[d*(4*e)+4*l+3]>0){o=Math.max(o,l);break t}t:for(l=0;l<o;l++)for(d=0;d<s;d++)if(r[d*(4*e)+4*l+3]>0){i=Math.min(l,i);break t}t:for(d=0;d<n;d++)for(l=0;l<e;l++)if(r[d*(4*e)+4*l+3]>0){a=Math.min(a,d);break t}return{x:i,y:a,maxX:o,maxY:n,w:o-i,h:n-a}}function r(t){switch(t){case"-":return"#f00";case"+":return"#0f0";default:return"white"}}function i(t,e,s,r,i,a){var o=a/2-r/2;return o<0&&(o=0),r>0&&e.width>0&&t.drawImage(e,0,0,r>e.width?e.width:r,e.height,o,i,Math.min(r,e.width),e.height),r=5,i+=e.height,s.clearRect(0,0,e.width,e.height),[r,i]}function a(t){var e;if("/"===t[1]){if("</b>"===(e=t.slice(0,4)))return{token:e,bold:-1,italic:0};if("</i>"===e)return{token:e,bold:0,italic:-1}}else{if("<b>"===(e=t.slice(0,3)))return{token:e,bold:1,italic:0};if("<i>"===e)return{token:e,bold:0,italic:-1}}return{token:"",bold:0,italic:0}}function o(t,e){var s=3*Math.pow(1-e,2)*(t[1].x-t[0].x)+6*(1-e)*e*(t[2].x-t[1].x)+3*Math.pow(e,2)*(t[3].x-t[2].x),r=3*Math.pow(1-e,2)*(t[1].y-t[0].y)+6*(1-e)*e*(t[2].y-t[1].y)+3*Math.pow(e,2)*(t[3].y-t[2].y);return{x:Math.pow(1-e,3)*t[0].x+3*Math.pow(1-e,2)*e*t[1].x+3*(1-e)*Math.pow(e,2)*t[2].x+Math.pow(e,3)*t[3].x,y:Math.pow(1-e,3)*t[0].y+3*Math.pow(1-e,2)*e*t[1].y+3*(1-e)*Math.pow(e,2)*t[2].y+Math.pow(e,3)*t[3].y,r:Math.atan2(r,s)}}var n,h=function(){function t(){this.name="WEB",this.Image=Image,this.Promise=Promise,this.buffers=[]}return t.prototype.getBuffer=function(t,e,s){var r;if(this.buffers.length){if(t){for(var i=0;i<this.buffers.length;i++)if(this.buffers[i].width===t&&this.buffers[i].height===e){r=this.buffers.splice(i,1)[0];break}}else r=this.buffers.pop();if(r)return s&&r.getContext("2d").clearRect(0,0,r.width,r.height),r}return r=document.createElement("canvas"),t&&(r.width=t,r.height=e),r},t.prototype.freeBuffer=function(t){this.buffers.push(t)},t.prototype.loadAsset=function(t,e,s,r){t.crossOrigin="Anonymous",t.addEventListener("load",s),t.addEventListener("error",r),t.src=e},t.prototype.requestAnimationFrame=function(t){window.requestAnimationFrame(t)},t}();!function(t){t[t.INVALID=0]="INVALID",t[t.DEATHKNIGHT=1]="DEATHKNIGHT",t[t.DRUID=2]="DRUID",t[t.HUNTER=3]="HUNTER",t[t.MAGE=4]="MAGE",t[t.PALADIN=5]="PALADIN",t[t.PRIEST=6]="PRIEST",t[t.ROGUE=7]="ROGUE",t[t.SHAMAN=8]="SHAMAN",t[t.WARLOCK=9]="WARLOCK",t[t.WARRIOR=10]="WARRIOR",t[t.DREAM=11]="DREAM",t[t.NEUTRAL=12]="NEUTRAL"}(n||(n={}));var d;!function(t){t[t.INVALID=0]="INVALID",t[t.MURLOC=14]="MURLOC",t[t.DEMON=15]="DEMON",t[t.MECHANICAL=17]="MECHANICAL",t[t.ELEMENTAL=18]="ELEMENTAL",t[t.PET=20]="PET",t[t.BEAST=20]="BEAST",t[t.TOTEM=21]="TOTEM",t[t.PIRATE=23]="PIRATE",t[t.DRAGON=24]="DRAGON"}(d||(d={}));var l;!function(t){t[t.INVALID=0]="INVALID",t[t.COMMON=1]="COMMON",t[t.FREE=2]="FREE",t[t.RARE=3]="RARE",t[t.EPIC=4]="EPIC",t[t.LEGENDARY=5]="LEGENDARY"}(l||(l={}));var u;!function(t){t[t.INVALID=0]="INVALID",t[t.GRIMY_GOONS=1]="GRIMY_GOONS",t[t.JADE_LOTUS=2]="JADE_LOTUS",t[t.KABAL=3]="KABAL"}(u||(u={}));var y;!function(t){t[t.INVALID=0]="INVALID",t[t.CORE=2]="CORE",t[t.EXPERT1=3]="EXPERT1",t[t.HOF=4]="HOF",t[t.NAXX=12]="NAXX",t[t.GVG=13]="GVG",t[t.BRM=14]="BRM",t[t.TGT=15]="TGT",t[t.LOE=20]="LOE",t[t.KARA=23]="KARA",t[t.OG=21]="OG",t[t.GANGS=25]="GANGS",t[t.UNGORO=27]="UNGORO",t[t.ICECROWN=1001]="ICECROWN",t[t.LOOTAPALOOZA=1004]="LOOTAPALOOZA"}(y||(y={}));var c;!function(t){t[t.INVALID=0]="INVALID",t[t.HERO=3]="HERO",t[t.MINION=4]="MINION",t[t.SPELL=5]="SPELL",t[t.WEAPON=7]="WEAPON",t[t.HERO_POWER=10]="HERO_POWER"}(c||(c={}));var x={};x[d.MURLOC]={enUS:"Murloc"},x[d.MECHANICAL]={enUS:"Mech"},x[d.ELEMENTAL]={enUS:"Elemental"},x[d.BEAST]={enUS:"Beast"},x[d.DEMON]={enUS:"Demon"},x[d.PIRATE]={enUS:"Pirate"},x[d.DRAGON]={enUS:"Dragon"},x[d.TOTEM]={enUS:"Totem"};var m=function(){function t(t,s){if(this.sunwell=t,!s)throw new Error("No card properties given");this.type=s.type,this.cost=s.cost||0,this.attack=s.attack||0,this.health=s.health||0,this.costsHealth=s.costsHealth||!1,this.hideStats=s.hideStats,this.language=s.language||"enUS",this.name=s.name||"",this.multiClassGroup=e(s.multiClassGroup,u),this.type=e(s.type,c),this.race=e(s.race,d);var i=e(s.set,y);this.set=this.getCardSet(i);var a=e(s.rarity,l);this.rarity=this.getRarityGem(a,i,this.type);var o=e(s.cardClass,n);this.cardClass=this.getCardFrameClass(o),this.type===c.MINION&&(this.raceText=s.raceText||this.getRaceText(),this.silenced=s.silenced||!1),this.type===c.WEAPON&&s.durability?this.health=s.durability:this.type===c.HERO&&s.armor&&(this.health=s.armor),this.costTextCoords={x:115,y:174},this.opposing=s.opposing||!1,this.elite=s.elite||!1,this.costColor=r(s.costStyle),this.attackColor=r(s.costStyle),this.healthColor=r(s.healthStyle),this.bodyText=s.collectionText||s.text||"",this.titleFont=t.options.titleFont,this.texture=s.texture,this.propsJson=JSON.stringify(s)}return t.prototype.initRender=function(t,e,s){this.width=t,this.target=e,this.callback=s,this.cacheKey=this.checksum(),this.key=this.cacheKey,this.updateAssets()},t.prototype.getAssetsToLoad=function(){var t=[this.cardFrameAsset];return this.cardFoundationAsset&&t.push(this.cardFoundationAsset),this.costGemAsset&&t.push(this.costGemAsset),this.nameBannerAsset&&t.push(this.nameBannerAsset),this.hideStats||(this.attackGemAsset&&t.push(this.attackGemAsset),this.healthGemAsset&&t.push(this.healthGemAsset)),this.elite&&this.dragonAsset&&t.push(this.dragonAsset),this.raceText&&t.push(this.raceBannerAsset),this.silenced&&t.push("silence-x"),this.multiBannerAsset&&t.push(this.multiBannerAsset),this.rarityGemAsset&&t.push(this.rarityGemAsset),this.watermarkAsset&&t.push(this.watermarkAsset),t},t.prototype.getCardArtTexture=function(){if(this.texture){if(this.texture instanceof this.sunwell.platform.Image)return this.texture;if("string"==typeof this.texture)return this.sunwell.assets[this.texture];var t=this.sunwell.getBuffer(this.texture.crop.w,this.texture.crop.h);return t.getContext("2d").drawImage(this.texture.image,this.texture.crop.x,this.texture.crop.y,this.texture.crop.w,this.texture.crop.h,0,0,t.width,t.height),t}return this.sunwell.log("No card texture specified. Creating empty texture."),this.sunwell.getBuffer(1024,1024)},t.prototype.getRaceText=function(){return this.race&&this.type===c.MINION&&this.race in x?x[this.race][this.language]:""},t.prototype.draw=function(t,e){var s=this,r=this.width/670,i=setTimeout(function(){s.sunwell.error("Drawing timed out",s.name)},this.sunwell.options.drawTimeout);if(e.save(),e.clearRect(0,0,t.width,t.height),this.sunwell.renderCache[this.cacheKey])this.sunwell.log("Skipping skeleton draw"),e.drawImage(this.sunwell.renderCache[this.cacheKey],0,0);else{if(this.drawCardArt(e,r),this.cardFoundationAsset&&this.drawCardFoundationAsset(e,r),this.drawFrameTexture(e,r),this.multiBannerAsset&&this.drawImage(e,this.multiBannerAsset,{dx:50,dy:119,ratio:r}),this.costGemAsset&&this.drawCostGem(e,r),this.rarityGemAsset&&this.drawRarityGem(e,r),this.nameBannerAsset&&this.drawNameBanner(e,r),this.raceText&&this.drawRaceBanner(e,r),this.drawAttackTexture(e,r),this.drawHealthTexture(e,r),this.elite&&this.dragonAsset){var a=this.dragonCoords;a.ratio=r,this.drawImage(e,this.dragonAsset,a)}if(this.watermarkAsset&&this.drawWatermark(e,r),this.sunwell.options.cacheSkeleton){var o=new this.sunwell.platform.Image;o.src=t.toDataURL(),this.sunwell.renderCache[this.cacheKey]=o}}this.drawName(e,r,this.name),this.drawStats(e,r),this.raceText&&this.drawRaceText(e,r,this.raceText),this.drawBodyText(e,r,!1,this.bodyText),this.silenced&&this.drawImage(e,"silence-x",{dx:166,dy:584,ratio:r}),e.restore(),clearTimeout(i),this.callback&&this.callback(t),this.target&&(this.target.src=t.toDataURL())},t.prototype.drawCardArt=function(t,e){var s=this.getCardArtTexture();t.save(),function(t,e,s){e.length<3||(t.beginPath(),t.moveTo(e[0].x*s,e[0].y*s),e.slice(1).forEach(function(e){t.lineTo(e.x*s,e.y*s)}),t.closePath(),t.stroke())}(t,this.artClipPolygon,e),t.clip(),t.fillStyle="grey",t.fillRect(0,0,670*e,1e3*e),t.drawImage(s,0,0,s.width,s.height,this.artCoords.dx*e,this.artCoords.dy*e,this.artCoords.dWidth*e,this.artCoords.dHeight*e),t.restore()},t.prototype.drawBodyText=function(t,e,r,o){var n=this,h="[x]"===o.substr(0,3),d=h?o.substr(3):o;if(d){var l,u,y,x=/(\d+)(.+?)\|4\((.+?),(.+?)\)/g,m=0,g=0,p=0,f=0,C=0,A=this.bodyTextCoords.dWidth,w=this.bodyTextCoords.dHeight,b=this.bodyTextCoords.dx+A/2,T=this.bodyTextCoords.dy+w/2;for(this.sunwell.options.debug&&(t.save(),t.strokeStyle="red",t.beginPath(),t.rect((b-A/2)*e,(T-w/2)*e,A*e,w*e),t.closePath(),t.stroke(),t.restore()),y=d;null!==(u=x.exec(d));)y=y.replace(u[0],u[1]+u[2]+(1===parseInt(u[1],10)?u[3]:u[4]));d=y,this.sunwell.log("Rendering body",d);var W=d.replace(/[\$#_]/g,"").replace(/\n/g," \n ").replace(/ +/g," ").split(/ /g),H=this.sunwell.getBuffer(A,w,!0),v=H.getContext("2d");v.fillStyle=this.bodyTextColor;var R=this.sunwell.options.bodyFontSize,E=this.sunwell.options.bodyLineHeight,O=d.replace(/<\/*.>/g,"").length,G=!1,k=d.replace(/<\/*.>/g,"");this.sunwell.log("counting length of "+k),this.sunwell.log("Length is "+O);var F=this.sunwell.getBuffer(H.width,E,!0),I=F.getContext("2d");if(I.fillStyle=this.bodyTextColor,h){var N=0;if(I.font=this.getFontMaterial(R,!1,!1),d.split("\n").forEach(function(t){var e=n.getLineWidth(I,R,t);e>N&&(N=e)}),N>H.width){var B=H.width/N;R*=B,E*=B}}else O>=65&&(R=.95*this.sunwell.options.bodyFontSize,E=.95*this.sunwell.options.bodyLineHeight),O>=80&&(R=.9*this.sunwell.options.bodyFontSize,E=.9*this.sunwell.options.bodyLineHeight),O>=100&&(R=.8*this.sunwell.options.bodyFontSize,E=.8*this.sunwell.options.bodyLineHeight);I.font=this.getFontMaterial(R,!!f,!!p),F.height=E,(r||O>=75&&this.type===c.SPELL)&&(G=!0);for(var L=0,M=W;L<M.length;L++){var S=M[L],P=S.trim().replace(/<((?!>).)*>/g,""),D=I.measureText(P).width;if(this.sunwell.log("Next word:",S),h||!(m+D>F.width||G&&m+D>.8*F.width)||l||(this.sunwell.log(m+D,">",F.width),this.sunwell.log("Calculated line break"),G=!1,l=!0,C++,m=(X=i(v,F,I,m,g,H.width))[0],g=X[1]),"\n"!==S){l=!1;for(var U=0;U<S.length;U++){var V=S[U];if("<"===V){var K=a(S.slice(U));if(K.token){U+=K.token.length-1,f+=K.bold,p+=K.italic;continue}}I.fillStyle=this.bodyTextColor,I.font=this.getFontMaterial(R,!!f,!!p),I.fillText(V,m+this.sunwell.options.bodyFontOffset.x,this.sunwell.options.bodyFontOffset.y),m+=I.measureText(V).width}m+=.275*I.measureText("M").width}else this.sunwell.log("Manual line break"),C++,m=(J=i(v,F,I,m,g,H.width))[0],g=J[1],l=!0,G=!1}if(C++,i(v,F,I,m,g,H.width),this.sunwell.freeBuffer(F),this.type!==c.SPELL||4!==C||G||r){var _=s(v);_.h=Math.ceil(_.h/F.height)*F.height,t.drawImage(H,_.x,_.y-2,_.w,_.h,(b-_.w/2)*e,(T-_.h/2)*e,_.w*e,(_.h+2)*e),this.sunwell.freeBuffer(H);var X,J}else this.drawBodyText(t,e,!0,this.bodyText)}},t.prototype.drawName=function(t,e,s){var r=460,i=160,a=335,n=612,h=this.sunwell.getBuffer(2*r,i,!0),d=h.getContext("2d"),l=this.nameTextCurve.maxWidth,u=this.nameTextCurve.curve;if(d.save(),this.sunwell.options.debug){d.lineWidth=2,d.strokeStyle="blue",d.fillStyle="red",d.beginPath(),d.moveTo(u[0].x,u[0].y),d.bezierCurveTo(u[1].x,u[1].y,u[2].x,u[2].y,u[3].x,u[3].y),d.stroke();for(var y=0,c=u;y<c.length;y++){var x=c[y];d.fillRect(x.x-4,x.y-4,8,8)}d.restore()}d.lineWidth=10,d.strokeStyle="black",d.lineCap="round",d.lineJoin="round",d.textAlign="left",d.textBaseline="middle";var m=function(){for(var t=[],e=d.measureText("M").width,r=0;r<s.length;r++){d.save();var i=s[r],a={x:1,y:1},o=d.measureText(i).width+.1*e;switch(i){case" ":o=.2*e;break;case"'":o=.27*e,a.x=.5,a.y=1}t[r]={width:o,scale:a},d.restore()}return t},g=45,p=[];do{g-=1,d.font=g+"px "+this.titleFont}while((p=m()).reduce(function(t,e){return t+e.width},0)>l&&g>10);for(var f,C,A=p.reduce(function(t,e){return t+e.width},0)/l,w=this.nameTextCurve.pathMiddle-A/2,b=A/s.length,T=0,W=0;W<s.length;W++){var H=s[W].trim(),v=p[W];if(0===T)T=(f=o(u,C=w+b*W)).x;else for(f=o(u,C+=.01);f.x<T;)f=o(u,C+=.001);H.length&&(d.save(),d.translate(f.x,f.y),v.scale.x&&d.scale(v.scale.x,v.scale.y),d.rotate(f.r),d.lineWidth=g/50*9,d.strokeStyle="black",d.fillStyle="black",d.fillText(H,0,0),d.strokeText(H,0,0),d.fillStyle="white",d.strokeStyle="white",d.lineWidth=g/50*2.5,d.fillText(H,0,0),d.restore()),T+=v.width}var R={sx:0,sy:0,sWidth:r,sHeight:i,dx:(a-r/2)*e,dy:(n-i)*e,dWidth:r*e,dHeight:i*e};t.drawImage(h,R.sx,R.sy,R.sWidth,R.sHeight,R.dx,R.dy,R.dWidth,R.dHeight),this.sunwell.freeBuffer(h)},t.prototype.drawNameBanner=function(t,e){var s=this.nameBannerCoords;s.ratio=e,this.drawImage(t,this.nameBannerAsset,s)},t.prototype.drawRaceBanner=function(t,e){var s=this.raceBannerCoords;s.ratio=e,this.drawImage(t,this.raceBannerAsset,s)},t.prototype.drawNumber=function(t,e,r,i,a,o,n){var h=this.sunwell.getBuffer(256,256,!0),d=h.getContext("2d"),l=a.toString().split(""),u=10;d.font=o+"px "+this.titleFont,d.lineCap="round",d.lineJoin="round",d.textAlign="left",d.textBaseline="hanging";for(var y=0,c=l;y<c.length;y++){var x=c[y];d.lineWidth=10,d.strokeStyle="black",d.fillStyle="black",d.fillText(x,u,10),d.strokeText(x,u,10),d.fillStyle=n,d.strokeStyle=n,d.lineWidth=2.5,d.fillText(x,u,10),u+=d.measureText(x).width}var m=s(d);t.drawImage(h,m.x,m.y,m.w,m.h,(e-m.w/2)*i,(r-m.h/2)*i,m.w*i,m.h*i),this.sunwell.freeBuffer(h)},t.prototype.drawRaceText=function(t,e,r){var i=this.sunwell.getBuffer(300,60,!0),a=i.getContext("2d"),o=10,n=r.split("");a.font="40px "+this.titleFont,a.lineCap="round",a.lineJoin="round",a.textBaseline="hanging",a.textAlign="left";for(var h=a.measureText("x").width,d=0,l=n;d<l.length;d++){var u=l[d];a.lineWidth=7,a.strokeStyle="black",a.fillStyle="black",a.fillText(u,o,10),a.strokeText(u,o,10),a.fillStyle="white",a.strokeStyle="white",a.lineWidth=1,a.fillText(u,o,10),o+=a.measureText(u).width,o+=.1*h}var y=s(a);t.drawImage(i,y.x,y.y,y.w,y.h,(this.raceTextCoords.dx-y.w/2)*e,(this.raceTextCoords.dy-y.h/2)*e,y.w*e,y.h*e),this.sunwell.freeBuffer(i)},t.prototype.drawCostGem=function(t,e){var s={x:47,y:105};this.costsHealth&&(s.x=43,s.y=58),this.drawImage(t,this.costGemAsset,{dx:s.x,dy:s.y,ratio:e})},t.prototype.drawRarityGem=function(t,e){var s=this.rarityGemCoords;s.ratio=e,this.drawImage(t,this.rarityGemAsset,s)},t.prototype.drawStats=function(t,e){this.hideStats||(this.drawNumber(t,this.costTextCoords.x,this.costTextCoords.y,e,this.cost,130,this.costColor),this.type!==c.HERO_POWER&&(this.attackTextCoords&&this.drawNumber(t,this.attackTextCoords.x,this.attackTextCoords.y,e,this.attack,124,this.attackColor),this.healthTextCoords&&this.drawNumber(t,this.healthTextCoords.x,this.healthTextCoords.y,e,this.health,124,this.healthColor)))},t.prototype.drawHealthTexture=function(t,e){if(!this.hideStats&&this.healthGemAsset){var s=this.healthGemCoords;s.ratio=e,this.drawImage(t,this.healthGemAsset,s)}},t.prototype.drawAttackTexture=function(t,e){if(!this.hideStats&&this.attackGemAsset){var s=this.attackGemCoords;s.ratio=e,this.drawImage(t,this.attackGemAsset,s)}},t.prototype.drawFrameTexture=function(t,e){var s=this.baseCardFrameCoords;s.ratio=e,this.drawImage(t,this.cardFrameAsset,s)},t.prototype.drawCardFoundationAsset=function(t,e){var s=this.cardFoundationCoords;s.ratio=e,this.drawImage(t,this.cardFoundationAsset,s)},t.prototype.drawWatermark=function(t,e){var s=this.getWatermarkCoords();s.ratio=e,this.type!==c.HERO_POWER&&(this.premium||this.type===c.MINION||this.type===c.HERO?(t.globalCompositeOperation="multiply",t.globalAlpha=.6):this.type===c.SPELL?(t.globalCompositeOperation="multiply",t.globalAlpha=.7):this.type===c.WEAPON&&(t.globalCompositeOperation="lighten",t.globalAlpha=.1),this.drawImage(t,this.watermarkAsset,s),t.globalCompositeOperation="source-over",t.globalAlpha=1)},t.prototype.getFontMaterial=function(t,e,s){var r,i=e?"bold":"",a=s?"italic":"",o=t+"px";return""!==this.sunwell.options.bodyLineStyle&&(o=o+"/"+this.sunwell.options.bodyLineStyle),r=e&&s?this.sunwell.options.bodyFontBoldItalic:e?this.sunwell.options.bodyFontBold:s?this.sunwell.options.bodyFontItalic:this.sunwell.options.bodyFontRegular,[i,a,o,'"'+r+'", sans-serif'].join(" ")},t.prototype.getLineWidth=function(t,e,s){for(var r=0,i=0,o=0,n=0,h=s.split(" ");n<h.length;n++){for(var d=h[n],l=0;l<d.length;l++){var u=d[l];if("<"===u){var y=a(d.slice(l));if(y.token){l+=y.token.length-1,i+=y.bold,o+=y.italic,t.font=this.getFontMaterial(e,!!i,!!o);continue}}t.fillText(u,r+this.sunwell.options.bodyFontOffset.x,this.sunwell.options.bodyFontOffset.y),r+=t.measureText(u).width}r+=.275*t.measureText("M").width}return r},t.prototype.checksum=function(){for(var t=this.propsJson+this.width+this.premium,e=t.length,s=0,r=0;r<e;r++){s=(s<<5)-s+t.charCodeAt(r),s|=0}return s},t.prototype.updateAssets=function(){this.type===c.HERO_POWER?this.cardFrameAsset=this.baseCardFrameAsset+(this.opposing?"opponent":"player"):this.cardFrameAsset=this.baseCardFrameAsset+n[this.cardClass].toLowerCase(),this.rarity&&this.baseRarityGemAsset&&(this.rarityGemAsset=this.baseRarityGemAsset+l[this.rarity].toLowerCase()),this.type===c.HERO_POWER?this.costGemAsset=null:this.costsHealth?this.costGemAsset="cost-health":this.costGemAsset="cost-mana",this.multiClassGroup&&(this.multiBannerAsset="multi-"+u[this.multiClassGroup].toLowerCase()),this.set&&(this.watermarkAsset="set-"+y[this.set].toLowerCase())},t.prototype.drawImage=function(t,e,s){var r=this.sunwell.getAsset(e);if(r){var i=s.ratio||1,a=s.sWidth||r.width,o=s.sHeight||r.height;t.drawImage(r,s.sx||0,s.sy||0,a,o,s.dx*i,s.dy*i,(s.dWidth||a)*i,(s.dHeight||o)*i)}else this.sunwell.error("Not drawing asset",e)},t.prototype.getCardFrameClass=function(t){switch(t){case n.DREAM:return n.HUNTER;case n.INVALID:return n.NEUTRAL;default:return t}},t.prototype.getCardSet=function(t){switch(t){case y.EXPERT1:case y.NAXX:case y.GVG:case y.BRM:case y.TGT:case y.LOE:case y.OG:case y.KARA:case y.GANGS:case y.UNGORO:case y.ICECROWN:case y.HOF:case y.LOOTAPALOOZA:return t;default:return null}},t.prototype.getRarityGem=function(t,e,s){switch(t){case l.INVALID:case l.FREE:return s===c.HERO?l.COMMON:null;case l.COMMON:if(e===y.CORE)return null}return t},t}(),g=function(e){function s(t,s){var r=e.call(this,t,s)||this;return r.premium=!1,r.bodyTextColor="black",r.bodyTextCoords={dx:143,dy:627,dWidth:376,dHeight:168,sWidth:376,sHeight:168},r.cardFoundationAsset=null,r.cardFoundationCoords=null,r.baseCardFrameAsset="frame-hero-",r.baseCardFrameCoords={sWidth:527,sHeight:795,dx:70,dy:87,dWidth:527,dHeight:795},r.baseRarityGemAsset="rarity-",r.nameBannerAsset="name-banner-hero",r.dragonAsset="elite-hero",r.dragonCoords={sWidth:444,sHeight:298,dx:172,dy:40,dWidth:444,dHeight:298},r.attackGemAsset=null,r.healthGemAsset="armor",r.attackGemCoords=null,r.attackTextCoords=null,r.healthGemCoords={sWidth:115,sHeight:135,dx:498,dy:752,dWidth:115,dHeight:135},r.healthTextCoords={x:554,y:822},r.nameBannerCoords={sWidth:490,sHeight:122,dx:91,dy:458,dWidth:490,dHeight:122},r.rarityGemCoords={dx:311,dy:529},r.raceBannerAsset=null,r.raceBannerCoords=null,r.raceTextCoords=null,r.nameTextCurve={pathMiddle:.5,maxWidth:420,curve:[{x:24,y:98},{x:170,y:36},{x:294,y:36},{x:438,y:96}]},r.artCoords={sWidth:346,sHeight:346,dx:161,dy:137,dWidth:346,dHeight:346},r.artClipPolygon=[{x:334,y:134},{x:369,y:143},{x:406,y:164},{x:435,y:187},{x:453,y:213},{x:469,y:245},{x:479,y:270},{x:481,y:290},{x:483,y:332},{x:483,y:380},{x:483,y:438},{x:484,y:485},{x:435,y:473},{x:389,y:467},{x:346,y:465},{x:297,y:466},{x:240,y:473},{x:185,y:486},{x:184,y:445},{x:182,y:357},{x:184,y:302},{x:188,y:271},{x:198,y:240},{x:210,y:217},{x:222,y:198},{x:239,y:178},{x:262,y:160},{x:291,y:145}],r.type!==c.HERO||s.armor||(r.healthGemAsset="health",r.healthGemCoords={sWidth:109,sHeight:164,dx:504,dy:728,dWidth:109,dHeight:164},r.healthTextCoords={x:556,y:825}),r}return t(s,e),s.prototype.getWatermarkCoords=function(){return{dx:247,dy:625,dWidth:170,dHeight:170}},s}(m),p=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!0,t.bodyTextColor="white",t.bodyTextCoords={dx:140,dy:627,dWidth:376,dHeight:168,sWidth:376,sHeight:168},t.cardFoundationAsset="base-hero-premium",t.cardFoundationCoords={dx:66,dy:84,dWidth:527,dHeight:799,sWidth:527,sHeight:799},t.baseCardFrameAsset="frame-hero-premium-",t.baseCardFrameCoords={dx:220,dy:838,dWidth:223,dHeight:45,sWidth:223,sHeight:45},t.nameBannerAsset="name-banner-hero-premium",t.nameBannerCoords={dx:87,dy:456,dWidth:490,dHeight:122,sWidth:490,sHeight:122},t.rarityGemCoords={dx:307,dy:528},t.dragonAsset="elite-hero-premium",t}return t(s,e),s}(g),f=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!1,t.bodyTextColor="black",t.bodyTextCoords={dx:144,dy:606,dWidth:380,dHeight:174,sWidth:380,sHeight:174},t.cardFoundationAsset=null,t.cardFoundationCoords=null,t.baseCardFrameAsset="hero-power-",t.baseCardFrameCoords={sWidth:564,sHeight:841,dx:56,dy:65,dWidth:564,dHeight:841},t.costTextCoords={x:338,y:124},t.baseRarityGemAsset=null,t.nameBannerAsset=null,t.dragonAsset=null,t.dragonCoords=null,t.attackGemAsset=null,t.healthGemAsset=null,t.attackGemCoords=null,t.attackTextCoords=null,t.healthGemCoords=null,t.healthTextCoords=null,t.nameBannerCoords=null,t.rarityGemCoords=null,t.raceBannerAsset=null,t.raceBannerCoords=null,t.raceTextCoords=null,t.nameTextCurve={pathMiddle:.54,maxWidth:440,curve:[{x:10,y:37},{x:110,y:37},{x:350,y:37},{x:450,y:37}]},t.artCoords={sWidth:261,sHeight:261,dx:208,dy:163,dWidth:261,dHeight:261},t.artClipPolygon=[{x:344,y:161},{x:264,y:173},{x:204,y:257},{x:207,y:331},{x:234,y:394},{x:333,y:431},{x:424,y:407},{x:465,y:355},{x:471,y:261},{x:427,y:187}],t}return t(s,e),s.prototype.getWatermarkCoords=function(){return{dx:0,dy:0,dWidth:0,dHeight:0}},s}(m),C=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!0,t.baseCardFrameAsset="hero-power-premium-",t.baseCardFrameCoords={dx:56,dy:60,dWidth:564,dHeight:850,sWidth:564,sHeight:850},t.costTextCoords={x:338,y:130},t}return t(s,e),s}(f),A=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!1,t.bodyTextColor="black",t.bodyTextCoords={dx:130,dy:622,dWidth:408,dHeight:176,sWidth:408,sHeight:176},t.cardFoundationAsset=null,t.cardFoundationCoords=null,t.baseCardFrameAsset="frame-minion-",t.baseCardFrameCoords={sWidth:528,sHeight:793,dx:70,dy:89,dWidth:528,dHeight:793},t.baseRarityGemAsset="rarity-minion-",t.nameBannerAsset="name-banner-minion",t.dragonAsset="elite-minion",t.dragonCoords={sWidth:436,sHeight:325,dx:188,dy:52,dWidth:436,dHeight:325},t.attackGemAsset="attack-minion",t.healthGemAsset="health",t.attackGemCoords={sWidth:154,sHeight:173,dx:36,dy:721,dWidth:154,dHeight:173},t.attackTextCoords={x:125,y:824},t.healthGemCoords={sWidth:109,sHeight:164,dx:504,dy:728,dWidth:109,dHeight:164},t.healthTextCoords={x:556,y:825},t.nameBannerCoords={sWidth:485,sHeight:113,dx:96,dy:469,dWidth:485,dHeight:113},t.raceBannerAsset="race-banner",t.raceBannerCoords={dx:129,dy:791,dWidth:408,dHeight:69,sWidth:408,sHeight:69},t.raceTextCoords={dx:337,dy:829},t.rarityGemCoords={dx:263,dy:532},t.nameTextCurve={pathMiddle:.55,maxWidth:450,curve:[{x:0,y:88},{x:98,y:112},{x:294,y:13},{x:460,y:80}]},t.artCoords={sWidth:461,sHeight:461,dx:105,dy:100,dWidth:461,dHeight:461},t.artClipPolygon=[{x:335,y:102},{x:292,y:110},{x:256,y:131},{x:222,y:163},{x:195,y:203},{x:171,y:273},{x:163,y:330},{x:170,y:398},{x:200,y:474},{x:266,y:547},{x:302,y:563},{x:343,y:567},{x:406,y:544},{x:449,y:506},{x:488,y:432},{x:505,y:346},{x:494,y:255},{x:460,y:172},{x:425,y:135},{x:385,y:111}],t}return t(s,e),s.prototype.getWatermarkCoords=function(){var t=604;return this.raceText&&(t-=10),{dx:231,dy:t,dWidth:225,dHeight:225}},s}(m),w=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!0,t.bodyTextColor="white",t.cardFoundationAsset="base-minion-premium",t.cardFoundationCoords={dx:66,dy:64,dwidth:528,dheight:818,swidth:528,sheight:818},t.baseCardFrameAsset="frame-minion-premium-",t.baseCardFrameCoords={dx:223,dy:553,sWidth:231,sHeight:329,dWidth:231,dHeight:329},t.baseRarityGemAsset="rarity-minion-premium-",t.rarityGemCoords={dx:245,dy:528},t.nameBannerAsset="name-banner-minion-premium",t.dragonAsset="elite-minion-premium",t.dragonCoords={dx:172,dy:17,sWidth:485,sHeight:341,dWidth:485,dHeight:341},t.attackGemAsset="attack-minion-premium",t.healthGemAsset="health-premium",t.raceBannerAsset="race-banner-premium",t.raceBannerCoords={dx:139,dy:779,dWidth:408,dHeight:81,sWidth:408,sHeight:81},t.raceTextCoords={dx:347,dy:826},t}return t(s,e),s}(A),b=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!1,t.bodyTextColor="black",t.bodyTextCoords={dx:144,dy:630,dWidth:378,dHeight:168,sWidth:378,sHeight:168},t.cardFoundationAsset=null,t.cardFoundationCoords=null,t.baseCardFrameAsset="frame-spell-",t.baseCardFrameCoords={dx:70,dy:133,dWidth:527,dHeight:746},t.baseRarityGemAsset="rarity-spell-",t.nameBannerAsset="name-banner-spell",t.dragonAsset="elite-spell",t.dragonCoords={sWidth:476,sHeight:259,dx:185,dy:91,dWidth:476,dHeight:259},t.attackGemAsset=null,t.healthGemAsset=null,t.attackGemCoords=null,t.attackTextCoords=null,t.healthGemCoords=null,t.healthTextCoords=null,t.nameBannerCoords={sWidth:507,sHeight:155,dx:80,dy:457,dWidth:507,dHeight:155},t.raceBannerAsset=null,t.raceBannerCoords=null,t.raceTextCoords=null,t.rarityGemCoords={sWidth:116,sHeight:77,dx:272,dy:541,dWidth:116,dHeight:77},t.nameTextCurve={pathMiddle:.49,maxWidth:450,curve:[{x:10,y:78},{x:170,y:36},{x:294,y:36},{x:450,y:80}]},t.artCoords={sWidth:418,sHeight:418,dx:123,dy:138,dWidth:418,dHeight:418},t.artClipPolygon=[{x:338,y:171},{x:425,y:179},{x:544,y:213},{x:551,y:474},{x:439,y:511},{x:327,y:519},{x:202,y:505},{x:118,y:474},{x:116,y:213},{x:236,y:176}],t}return t(s,e),s.prototype.getWatermarkCoords=function(){return{dx:232,dy:612,dWidth:210,dHeight:210}},s}(m),T=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!0,t.bodyTextColor="white",t.bodyTextCoords={dx:152,dy:634,dWidth:366,dHeight:168,sWidth:366,sHeight:168},t.cardFoundationAsset="base-spell-premium",t.cardFoundationCoords={dx:52,dy:125,dwidth:580,dheight:755,swidth:580,sheight:755},t.baseCardFrameAsset="frame-spell-premium-",t.baseCardFrameCoords={dx:220,dy:126,sWidth:226,sHeight:754,dWidth:226,dHeight:754},t.nameBannerCoords={dx:84,dy:464,dWidth:497,dHeight:152,sWidth:497,sHeight:152},t.nameTextCurve={pathMiddle:.49,maxWidth:450,curve:[{x:10,y:86},{x:170,y:44},{x:294,y:44},{x:450,y:88}]},t.baseRarityGemAsset="rarity-spell-premium-",t.rarityGemCoords={dx:283,dy:545,dWidth:107,dHeight:74,sWidth:107,sHeight:74},t.nameBannerAsset="name-banner-spell-premium",t.dragonAsset="elite-spell-premium",t.dragonCoords={dx:185,dy:91,sWidth:476,sHeight:259,dWidth:476,dHeight:259},t}return t(s,e),s}(b),W=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!1,t.bodyTextColor="white",t.bodyTextCoords={dx:146,dy:628,dWidth:388,dHeight:168,sWidth:388,sHeight:168},t.cardFoundationAsset=null,t.cardFoundationCoords=null,t.baseCardFrameAsset="frame-weapon-",t.baseCardFrameCoords={sWidth:527,sHeight:775,dx:80,dy:103,dWidth:527,dHeight:775},t.nameBannerAsset="name-banner-weapon",t.baseRarityGemAsset="rarity-weapon-",t.dragonAsset="elite-weapon",t.dragonCoords={dx:184,dy:89,dWidth:455,dHeight:315,sWidth:455,sHeight:315},t.attackGemAsset="attack-weapon",t.healthGemAsset="durability",t.attackGemCoords={sWidth:135,sHeight:133,dx:65,dy:753,dWidth:135,dHeight:133},t.attackTextCoords={x:136,y:820},t.healthGemCoords={sWidth:126,sHeight:140,dx:501,dy:744,dWidth:126,dHeight:140},t.healthTextCoords={x:563,y:819},t.nameBannerCoords={sWidth:514,sHeight:108,dx:87,dy:468,dWidth:514,dHeight:108},t.rarityGemCoords={sWidth:96,sHeight:90,dx:302,dy:520,dWidth:96,dHeight:90},t.raceBannerAsset=null,t.raceBannerCoords=null,t.raceTextCoords=null,t.nameTextCurve={pathMiddle:.56,maxWidth:450,curve:[{x:18,y:56},{x:66,y:56},{x:400,y:56},{x:456,y:56}]},t.artCoords={sWidth:384,sHeight:384,dx:152,dy:135,dWidth:384,dHeight:384},t.artClipPolygon=[{x:352,y:139},{x:418,y:155},{x:469,y:188},{x:497,y:222},{x:523,y:267},{x:533,y:315},{x:531,y:366},{x:514,y:420},{x:485,y:461},{x:444,y:496},{x:375,y:515},{x:309,y:515},{x:236,y:484},{x:192,y:434},{x:160,y:371},{x:158,y:303},{x:173,y:246},{x:203,y:201},{x:242,y:167},{x:287,y:148}],t}return t(s,e),s.prototype.getWatermarkCoords=function(){return{dx:241,dy:599,dWidth:220,dHeight:220}},s}(m),H=function(e){function s(){var t=null!==e&&e.apply(this,arguments)||this;return t.premium=!0,t.cardFoundationAsset="base-weapon-premium",t.cardFoundationCoords={dx:75,dy:101,dWidth:527,dHeight:778,sWidth:527,sHeight:778},t.baseCardFrameAsset="frame-weapon-premium-",t.baseCardFrameCoords={dx:229,dy:532,dWidth:226,dHeight:347,sWidth:226,sHeight:347},t.nameBannerAsset="name-banner-weapon-premium",t.attackGemAsset="attack-weapon-premium",t.healthGemAsset="durability-premium",t.dragonAsset="elite-weapon-premium",t.dragonCoords={dx:164,dy:69,dWidth:489,dHeight:334,sWidth:489,sHeight:334},t}return t(s,e),s}(W);return function(){function t(t){t.titleFont=t.titleFont||"Belwe",t.bodyFontRegular=t.bodyFontRegular||"Franklin Gothic",t.bodyFontBold=t.bodyFontBold||t.bodyFontRegular,t.bodyFontItalic=t.bodyFontItalic||t.bodyFontRegular,t.bodyFontBoldItalic=t.bodyFontBoldItalic||t.bodyFontRegular,t.aspectRatio=t.aspectRatio||1.492537,t.bodyFontSize=t.bodyFontSize||60,t.bodyFontOffset=t.bodyFontOffset||{x:0,y:0},t.bodyLineHeight=t.bodyLineHeight||50,t.bodyLineStyle=t.bodyLineStyle||"1em",t.assetFolder=t.assetFolder||"/assets/",t.drawTimeout=t.drawTimeout||5e3,t.cacheSkeleton=t.cacheSkeleton||!1,t.debug=t.debug||!1,this.platform=new h,this.options=t,this.assets={},this.assetListeners={},this.renderQuery={},this.renderCache={},this.isRendering=!1}return t.prototype.log=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];this.options.debug&&console.log.apply("[INFO]",arguments)},t.prototype.error=function(){for(var t=[],e=0;e<arguments.length;e++)t[e]=arguments[e];console.log.apply("[ERROR]",arguments)},t.prototype.fetchAsset=function(t){var e=this.assets,s=this.assetListeners,r=this;return new this.platform.Promise(function(i){void 0===e[t]?(e[t]=new r.platform.Image,r.log("Requesting",t),r.platform.loadAsset(e[t],t,function(){if(s[t]){for(var r=0,a=s[t];r<a.length;r++){(0,a[r])(e[t])}delete s[t]}i()},function(){r.error("Error loading asset:",t),i()})):e[t].complete?i():(s[t]=s[t]||[],s[t].push(i))})},t.prototype.getBuffer=function(t,e,s){return this.platform.getBuffer(t,e,s)},t.prototype.freeBuffer=function(t){return this.platform.freeBuffer(t)},t.prototype.render=function(){var t=this,e=Object.keys(this.renderQuery);if(e.length){var s=e[0],r=this.renderQuery[s];delete this.renderQuery[s];var i=r.canvas.getContext("2d");this.log("Preparing assets for",r.name);var a=[];r.texture&&"string"==typeof r.texture&&a.push(r.texture);for(var o=0,n=r.getAssetsToLoad();o<n.length;o++){var h=n[o],d=this.getAssetPath(h);this.assets[d]&&this.assets[d].complete||a.push(d)}this.log("Preparing to load assets");for(var l=[],u=0,y=a;u<y.length;u++){var c=y[u];l.push(this.fetchAsset(c))}this.platform.Promise.all(l).then(function(){var e=Date.now();r.draw(r.canvas,i),t.log(r,"finished drawing in "+(Date.now()-e)+"ms"),t.isRendering=!1,Object.keys(t.renderQuery).length&&t.renderTick()}).catch(function(e){t.error("Error while drawing card:",e),t.isRendering=!1})}},t.prototype.getAssetPath=function(t){return this.options.assetFolder+t+".png"},t.prototype.getAsset=function(t){var e=this.getAssetPath(t),s=this.assets[e];if(s){if(s.complete)return s;this.error("Attempting to getAsset not loaded",s,e)}else this.error("Missing asset",t,"at",e)},t.prototype.renderTick=function(){var t=this;this.isRendering=!0,this.platform.requestAnimationFrame(function(){return t.render()})},t.prototype.createCard=function(t,s,r,i,a){var o,n=Math.round(s*this.options.aspectRatio);i&&i instanceof HTMLCanvasElement?((o=i).width=s,o.height=n):o=this.getBuffer(s,n,!0);var h={};h[c.HERO]=r?p:g,h[c.MINION]=r?w:A,h[c.SPELL]=r?T:b,h[c.WEAPON]=r?H:W,h[c.HERO_POWER]=r?C:f;var d=e(t.type,c),l=h[d];if(!l)throw new Error("Got an unrenderable card type: "+d);var u=new l(this,t);return u.canvas=o,u.initRender(s,i,a),this.log("Queried render:",u.name),this.renderQuery[u.key]?this.log("Skipping",u.key,"(already queued)"):(this.renderQuery[u.key]=u,this.isRendering||this.renderTick()),u},t}()}();
export { Sunwell };
