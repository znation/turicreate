import * as React from 'react';
import { Component } from 'react';
import * as _style_dict from 'json!./index.scss.json';
import ClassNameMap from '../../../../../util/ClassNameMap';
const styles = ClassNameMap(_style_dict);

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
