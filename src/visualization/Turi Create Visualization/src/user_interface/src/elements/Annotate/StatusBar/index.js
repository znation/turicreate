import React, { Component } from 'react';
var style = require('json!./index.module.scss.json');

var swap = require('image!./assets/swap.svg');

class StatusBar extends Component {
  swapInfiniteScroll = () => {
    this.props.toggleInfiniteScroll(this);
    this.props.removeSelected();
  }
  render(){
    return (
      <div className={style.StatusBar}>
        <div className={style.StatusBarMetrics}>
            Total: {this.props.total}
        </div>
        <div>
            <img src={swap}
                 className={style.SwapButton}
                 onClick={this.swapInfiniteScroll.bind(this)}/>
            <div className={style.SwapButtonHelperText}>
                Hide Annotated image
            </div>
        </div>
      </div>
    );
  }
}

export default StatusBar;