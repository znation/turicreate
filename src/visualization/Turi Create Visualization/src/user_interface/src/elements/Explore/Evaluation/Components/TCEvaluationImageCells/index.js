import React, { Component } from 'react';
var styles = require('json!./index.scss.json');

import TCEvaluationImageCellsHover from './TCEvaluationImageCellsHover/index';

class TCEvaluationImageCells extends Component {
  constructor(props){
    super(props)
    this.state = {
      "hover": false
    }
  }

  onHoverState = () => {
    this.setState({"hover": true});
  }

  offHoverState = () => {
    this.setState({"hover": false});
  }

  renderImage = () => {
    if(this.state.hover){
      return (
        <TCEvaluationImageCellsHover src={"data:image/"+this.props.value.format+";base64,"+this.props.value.data}/>
      );
    }
  }

  render() {
    return (
      <div className={styles.TCEvaluationImageCellsWrapper}
           onMouseEnter={this.onHoverState.bind(this)}
           onMouseLeave={this.offHoverState.bind(this)}>
        <div className={styles.TCEvaluationImageCells}>
          <img width={30}
               src={"data:image/"+this.props.value.format+";base64,"+this.props.value.data} />
        </div>
        {this.renderImage()}
      </div>
    );
  }
}

export default TCEvaluationImageCells;
