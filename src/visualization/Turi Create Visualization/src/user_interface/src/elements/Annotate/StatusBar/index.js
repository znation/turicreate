import React, { Component } from 'react';
import style from 'json!./index.module.scss.json';

import swap from 'image!./assets/swap.svg';

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