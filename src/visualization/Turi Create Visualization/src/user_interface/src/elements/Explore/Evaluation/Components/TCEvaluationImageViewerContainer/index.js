import React, { Component } from 'react';
var styles = require('json!./index.scss.json');

import TCEvaluationImageComponent from '../TCEvaluationImageComponent/index.js';

class TCEvaluationImageViewerContainer extends Component {
  render() {
    return (
      <div className={styles.TCEvaluationImageViewerContainer}>
        <div className={styles.TCEvaluationImageViewerContainerImages}>
          { 
            this.props.data.images.map((data, index) => {
              return(
                <div className={styles.TCEvaluationImageComponentWrapper}>
                  <TCEvaluationImageComponent src={data} />
                </div>
              )
            })
          }
        </div>
      </div>
    );
  }
}

export default TCEvaluationImageViewerContainer;
