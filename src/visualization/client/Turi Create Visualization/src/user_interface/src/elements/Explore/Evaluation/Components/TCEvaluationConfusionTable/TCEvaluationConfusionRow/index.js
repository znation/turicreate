import * as React from 'react';
import { Component } from 'react';
import * as styles from 'json!./index.scss.json';

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
