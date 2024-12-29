import { html, css } from 'lit'
import { customElement, state } from 'lit/decorators.js'
import { Transport } from './transport';
import "./cards-list";
import { Element } from "./element";

@customElement("my-app")
export class App extends Element {
  static styles = css`
  .card-container {
    display: flex;
    flex-direction: column;
    align-items: center; /* Centers cards horizontally */
    gap: 16px; /* Adds spacing between cards */
  }
  :host {
    width: 100%;
    height: 100%;
  }
    `;
  @state()
  connected: boolean = false;

  constructor() {
    super();

    Transport.getInstance().addInitializeHandler(this, (data) => {
      document.title = data.title;
      this.connected = true;
    });

    Transport.getInstance().init();
  }

  protected render() {
    return html`
    ${this.connected
        ? html`<cards-list/>`
        : html`<p>Conntecing</p>`
      }
    `;
  }
};
