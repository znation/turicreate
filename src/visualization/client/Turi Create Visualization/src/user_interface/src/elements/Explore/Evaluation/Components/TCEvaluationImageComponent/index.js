import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.scss.json';
import ClassNameMap from '../../../../../util/ClassNameMap';
const styles = ClassNameMap(_style_dict);

import export_img from 'image!./assets/export.png';

class TCEvaluationImageComponent extends Component {
  constructor(props){
    super(props);
    this.imgEl = React.createRef();
    this.state = {
      "width": "auto",
      "height": "auto"
    }
  }

  setHeight = () =>{
    const imageWidth = this.imgEl.current.clientWidth;
    const imageHeight = this.imgEl.current.clientHeight;
    if(imageWidth > imageHeight){
      this.setState({"height" : 130})
    }else{
      this.setState({"width" : 130})
    }
  }

  render() {

    return (
      <div className={styles.TCEvaluationImageComponent}>
        <img src={"data:image/"+this.props.src.format+";base64,"+this.props.src.data}
             className={styles.TCEvaluationImageComponentImage}
             ref={this.imgEl}
             width={this.state.width}
             height={this.state.height}
             onLoad={this.setHeight.bind(this)}
             alt="" />
        <div className={styles.TCEvaluationImageComponentExport}>
          <img src={export_img}
               width={10}
               height={10} 
	       alt="Export" />
        </div>
      </div>
    );
  }
}

export default TCEvaluationImageComponent;
