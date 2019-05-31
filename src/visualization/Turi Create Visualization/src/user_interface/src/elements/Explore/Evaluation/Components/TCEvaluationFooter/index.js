import React, { Component } from 'react';
import * as d3 from 'd3';
var styles = require('json!./index.scss.json');

var down_arrow = require('image!./assets/down.svg');
var cancel = require('image!./assets/cancel.svg');
import TCEvaluationConfusionTable from '../TCEvaluationConfusionTable/index.js';
import TCEvaluationImageViewerContainer from '../TCEvaluationImageViewerContainer/index.js';

class TCEvaluationFooter extends Component {

  cssOpenStyleContainer = () => {
    if(this.props.footer_open){
      return {"min-height": parseInt((window.innerHeight/2), 10) + "px"}
    }
  }

  cssOpenStyleIcon = () => {
    if(this.props.footer_open){
      return {"transform":"rotate(-180deg)"}
    }
  }

  filterData = (element) => {
    return element.actual == this.props.selected_actual && element.predicted == this.props.selected_prediction;
  }

  filterErrorData = (element) => {
    if(this.props.filter_confusion != null){
      return element.actual == this.props.filter_confusion;
    }else{
      return true;
    }
  }

  render_table = () => {
    if((this.props.selected_actual != null) && (this.props.selected_prediction != null)){
      return (
        <div>
          <TCEvaluationImageViewerContainer reset={this.props.selectRowConfusions.bind(this, null, null)}
                                            data={this.props.considerations.filter(this.filterData)[0]}/>
        </div>
      );
    } else {
      return (
        <div>
          <TCEvaluationConfusionTable considerations={this.props.considerations}
                                      filter_confusion={this.props.filter_confusion}
                                      sort_by_confusions={this.props.sort_by_confusions}
                                      sort_direction_confusions={this.props.sort_direction_confusions}
                                      updateSortByConfusion={this.props.updateSortByConfusion.bind(this)}
                                      selectRowConfusions={this.props.selectRowConfusions.bind(this)}/>
        </div>
      );
    }
  }

  renderPill = (key, value) => {
    return (
      <div className={styles.TCEvaluationFooterLabelIcon}>
        <span>
          {key} :&nbsp;
        </span>
        {value}
        &nbsp;
        &nbsp;
        <img src={cancel} 
         onClick={this.resetFilter.bind(this)}/>
        </div>
    )
  }

  renderPredictedPill = (key, value) => {
    return (
      <div className={styles.TCEvaluationFooterLabelIcon}>
        <span>
          {key} :&nbsp;
        </span>
        {value}
        &nbsp;
        &nbsp;
        <img src={cancel} 
         onClick={this.props.selectRowConfusions.bind(this, null, null)}/>
        </div>
    )
  }

  resetFilter = () => {
    this.props.row_click(null);
  }

  renderLabel = () => {
    if(this.props.filter_confusion != null){
      return(
        <div className={styles.TCEvaluationFooterLabel}>
          {this.renderPill("Actual", this.props.filter_confusion)}
        </div>
      );
    }
  }

  render() {
    return (
      <div className={styles.TCEvaluationFooter}
           style={this.cssOpenStyleContainer()}>
        <div className={styles.TCEvaluationFooterContainer}>
          <div className={styles.TCEvaluationFooterText}>
            <div>
              <div className={styles.TCEvaluationErrorsPill}>
                {d3.sum(this.props.considerations
                                  .filter(this.filterErrorData)
                                  .map((element) => element.count))
                }
              </div>
              Errors
            </div>
            {this.renderLabel()}
            <div className={styles.TCEvaluationFooterLabel}>
              {(this.props.selected_prediction != null)?this.renderPredictedPill("Predicted", this.props.considerations.filter(this.filterData)[0].predicted):[]}
            </div>
          </div>
          <div className={styles.TCEvaluationFooterCarret}
               onClick={this.props.updateFooterOpen.bind(this)}>
            <img src={down_arrow}
                 className={styles.TCEvaluationFooterCarretImage}
                 style={this.cssOpenStyleIcon()}/>
          </div>
        </div>
        {this.render_table()}
      </div>
    );
  }
}

export default TCEvaluationFooter;
