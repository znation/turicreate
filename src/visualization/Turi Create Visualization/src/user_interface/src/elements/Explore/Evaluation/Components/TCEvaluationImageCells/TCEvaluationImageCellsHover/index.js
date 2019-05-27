import React, { Component } from 'react';
var css = require('json!./index.scss.json');

class TCEvaluationImageCellsHover extends Component {
  render() {
    return (
      <div className={css.TCEvaluationImageCellsHover}>
        <img width={200}
             src={this.props.src} />
      </div>
    );
  }
}

export default TCEvaluationImageCellsHover;
