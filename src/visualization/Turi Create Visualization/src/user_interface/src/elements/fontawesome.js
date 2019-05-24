import React, { Component } from 'react';

export default class FontAwesomeIcon extends Component {
  render() {
    return (
      <span className={this.props.icon} {...this.props}>{this.props.children}</span>
    );
  }
}