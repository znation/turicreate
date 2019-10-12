import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.module.scss.json';
import ClassNameMap from '../../../../util/ClassNameMap';
const style = ClassNameMap(_style_dict);

class SimilarImage extends Component {

  resizeImage(width, height) {
    if((width/height) > 1) {
      return {"width": "auto", "height": style.singleImgSize + "px", "top": "0px", "left": parseInt((-1*(((width/height)*style.singleImgSize)-style.singleImgSize)/2), 10) + "px"};
    } else {
      return {"width": style.singleImgSize + "px", "height": "auto", "top": parseInt((-1*(((height/width)*style.singleImgSize)-style.singleImgSize)/2), 10) + "px", "left": "0px"};
    }
  }

  render() {
    if (!this.props.selected) {
      return (
        <div className={`${style.SingleImageSimilarityImage}`}
             onClick={this.props.onClick.bind(this, this.props.src.index)}>
          <img src={this.props.src.src}
               className={style.SingleImageIndex}
               style={this.resizeImage(this.props.src.width, this.props.src.height)} 
	       alt="" />
        </div>
      );
    }else{
      return (
        <div className={`${style.SingleImageSimilarityImage} ${style.Selected}`}
             onClick={this.props.onClick.bind(this, this.props.src.index)}>
          <img src={this.props.src.src}
               className={style.SingleImageIndex}
               style={this.resizeImage(this.props.src.width, this.props.src.height)} 
	       alt="" />
        </div>
      );
    }
  }
}

export default SimilarImage;
