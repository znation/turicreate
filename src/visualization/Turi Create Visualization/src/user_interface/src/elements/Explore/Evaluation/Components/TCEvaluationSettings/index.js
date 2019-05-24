import React, { Component } from 'react';
var css = require('json!./index.scss.json');
var filter = require('image!./assets/filter.svg');

import TCEvaluationSettingsSelection from '../TCEvaluationSettingsSelection/index';

class TCEvaluationSettings extends Component {
  constructor(props){
    super(props)
    this.state = {
      "open": false
    }
  }

  changeOpenClose = () => {
    this.setState({"open": !this.state.open});
  }

  renderSettings = () => {
    if(this.state.open) {
      return (
        <div className={css.TCEvaluationSettingsSelectionContainer}>
          <TCEvaluationSettingsSelection accuracy={this.props.accuracy}
                                         changeAccuracy={this.props.changeAccuracy.bind(this)}
                                         precision={this.props.precision}
                                         changePrecision={this.props.changePrecision.bind(this)}
                                         recall={this.props.recall}
                                         changeRecall={this.props.changeRecall.bind(this)}
                                         f1_score={this.props.f1_score}
                                         changeF1Score={this.props.changeF1Score.bind(this)} />
        </div>
      );
    }
  }

  render() {
    return (
      <div className={css.TCEvaluationSettings}>
        <div className={css.TCEvaluationSettingsImage}
             onClick={this.changeOpenClose.bind(this)}>
          <img src={filter}
               height={20}
               width={20}/>
        </div>
        {this.renderSettings()}
      </div>
    );
  }
}

export default TCEvaluationSettings;
