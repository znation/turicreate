import React, { Component } from 'react';
var styles = require('json!./index.scss.json');

import TCEvaluationConfusionCell from './TCEvaluationConfusionCell/index.js';

class TCEvaluationConfusionRow extends Component {

  render() {
    return (
      <div className={styles.TCEvaluationConfusionRow} onClick={this.props.selectRowConfusions.bind(this, this.props.consideration.actual, this.props.consideration.predicted)}>
        <TCEvaluationConfusionCell type="text" 
                                   value={this.props.consideration.actual}/>
        <TCEvaluationConfusionCell type="text"
                                   value={this.props.consideration.predicted}/>
        <TCEvaluationConfusionCell type="amount"
                                   value={this.props.consideration.count}
                                   max={this.props.maxValue}/>
        <TCEvaluationConfusionCell type="images"
                                   value={this.props.consideration.images}/>
      </div>
    );
  }
}

export default TCEvaluationConfusionRow;
